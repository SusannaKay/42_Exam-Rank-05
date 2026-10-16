#include "bsq.h"
void free_function(t_map *map, t_square *square, int err, FILE *file)
{
	if (err)
		fprintf(stderr, "map error\n");
	if (map)
	{
		if (map->grid)
		{
			for (int i = 0; i < map->num_rows; i++)
			{
				free(map->grid[i]);
				map->grid[i] = NULL;
			}
			free(map->grid);
			map->grid = NULL;
		}
		free(map);
		map = NULL;
	}
	if (square)
	{
		free(square);
		square = NULL;
	}
	fclose(file);
	return;
}
int get_header(t_map *map, FILE *file)
{
	int params = 0;
	params = fscanf(file, "%d %c %c %c", &map->num_rows, &map->empty, &map->obstacle, &map->full);
	if (params != 4 || params == -1)
		return (1);
	char extra;
	params = fscanf(file, "%c", &extra);
	if (params != 1 || params == -1 || extra != '\n')
		return (1);
	if (map->num_rows < 1)
		return(1);
	if (map->empty < 32 || map->empty > 126)
		return(1);
	if (map->obstacle < 32 || map->obstacle > 126)
		return(1);
	if (map->full < 32 || map->full > 126)
		return(1);
	if (map->empty == map->full || map->empty == map->obstacle || map->obstacle == map->full)
		return(1);
	return (0);
}

int get_map(t_map *map, FILE *file){
	for (int i = 0; i < map->num_rows; i++){
		ssize_t byte_read = 0;
		size_t len = 0;
		byte_read = getline(&map->grid[i], &len, file);
		if (byte_read == -1)
			return(1);
		if ( i == 0)
			map->cols = byte_read - 1;
		else if (byte_read - 1 != map->cols) // check lunghezze tutte uguali 
			return (1);
		if (map->grid[i][byte_read - 1] != '\n') // tutte le righe finiscono con nl
			return(1);
	}
	char extra;
	if (fscanf(file, "%c", &extra) == 1) // check num righe mappa finite
		return (1);
	for (int i = 0; i < map->num_rows; i++) // check char empty e obstacle
	{
		for (int j = 0; j < map->cols; j++)
		{
			if (map->grid[i][j] != map->empty && map->grid[i][j] != map->obstacle)
				return (1);
		}
	}
	return (0);
}
int min_num(int a, int b, int c)
{
	if (a <= b && a <= c)
		return (a);
	if (b <= a && b <= c)
		return (b);
	return (c);
}
t_square *find_square(t_map *map, t_square *square){

	int **new_grid;
	new_grid = calloc(map->num_rows, sizeof(int *));
	if (!new_grid)
		return (NULL);
	for (int i = 0; i < map->num_rows; i++)
	{
		new_grid[i] = calloc(map->cols, sizeof(int));
		if (!new_grid[i])
		{
			for (int j = 0; j < i; j++)
			{
				free(new_grid[j]);
				new_grid[j] = NULL;
			}
			free(new_grid);
			new_grid = NULL;
			return(NULL);
		}
	}
	for (int i = 0; i < map->num_rows; i++)
	{
		for(int j = 0; j < map->cols; j++){
			if (map->grid[i][j] == map->obstacle)
				new_grid[i][j] = 0;
			else if (i == 0 || j == 0)
				new_grid[i][j] = 1;
			else
				new_grid[i][j] = min_num(new_grid[i - 1][j], new_grid[i][j - 1], new_grid[i -1][j -1]) + 1;
			if (new_grid[i][j] > square->size)
			{
				square->size = new_grid[i][j];
				square->col = j;
				square->row = i;
			}
		}
	}
	for (int i = 0; i < map->num_rows; i++)
	{
		free(new_grid[i]);
		new_grid[i] = NULL;
	}
	free(new_grid);
	new_grid = NULL;
	if (square->size == 0)
	{
		free(square);
		square = NULL;
	}
	return (square);
}

void print_function(t_square *square,t_map *map){
	for (int start_i = square->row - square->size + 1; start_i <= square->row; start_i++)
	{
		for (int start_j = square->col - square->size + 1; start_j <= square->col; start_j++)
			map->grid[start_i][start_j] = map->full;
	}
	for(int i = 0; i < map->num_rows; i++)
		fputs(map->grid[i], stdout);
}

int main(int argc, char **argv)
{
	FILE *file;
	if (argc == 1)
		file = stdin;
	else if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (!file)
		{
			fprintf(stderr, "map error\n");
			return (1);
		}
	}
	else
	{
		fprintf(stderr, "map error\n");
		return (1);
	}
	t_map *map;
	map = calloc(1, sizeof(t_map));
	if (!map)
		return (1);
	if (get_header(map, file) == 1)
		return (free_function(map, NULL, 1, file), 1);
	map->grid= calloc(map->num_rows, sizeof(char *));
	if (!map->grid)
		return(free_function(map, NULL, 0, file), 1);
	if (get_map(map, file) == 1)
		return (free_function(map, NULL, 1, file), 1);
	t_square *square;
	square = calloc(1, sizeof(t_square));
	if(!square)
		return(free_function(map, NULL, 0, file), 1);
	if (find_square(map, square) == NULL)
		return(free_function(map, NULL, 1, file), 1);
	print_function(square, map);
	free_function(map, square, 0, file);
	return (0);
}