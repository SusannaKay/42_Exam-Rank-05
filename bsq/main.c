#include "bsq.h"

void free_function(t_map *map, t_square *square)
{
	if (square)
	{
		free(square);
		square = NULL;
	}
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
}
int verify_header(t_map *map)
{
	if (map->num_rows <= 0)
		return (1);
	if (map->empty < 32 || map->empty > 126)
		return (1);
	if (map->full < 32 || map->full > 126)
		return (1);
	if (map->obstacle < 32 || map->obstacle > 126)
		return (1);
	if (map->obstacle == map->empty || map->obstacle == map->full || map->full == map->empty)
		return (1);
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

t_square *find_square(t_map *map)
{
	t_square *square = calloc(1, sizeof(t_square));
	if (!square)
		return (NULL);
	int **new_grid;
	new_grid = calloc(map->num_rows, sizeof(int *));
	if (!new_grid)
	{
		free(square);
		square = NULL;
		return (NULL);
	}
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
			free(square);
			square = NULL;
			return (square);
		}
	}
	for (int i = 0; i < map->num_rows; i++)
	{
		for (int j = 0; j < map->cols; j++)
		{
			if (map->grid[i][j] == map->obstacle)
				new_grid[i][j] = 0;
			else if (j == 0 || i == 0)
				new_grid[i][j] = 1;
			else
			{
				new_grid[i][j] = min_num(new_grid[i - 1][j], new_grid[i][j - 1], new_grid[i - 1][j - 1]) + 1;
			}
			if (new_grid[i][j] > square->size)
			{
				square->size = new_grid[i][j];
				square->row = i;
				square->col = j;
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

void print_square(t_square *square, t_map *map)
{
	int start_i = square->row - square->size + 1;
	int start_j = square->col - square->size + 1;
	for (int i = start_i; i <= square->row; i++)
	{
		for (int j = start_j; j <= square->col; j++)
		{
			map->grid[i][j] = map->full;
		}
	}
	for (int i = 0; i < map->num_rows; i++)
		fputs(map->grid[i], stdout);
}

int verify_map(t_map *map)
{
	for (int i = 0; i < map->num_rows; i++)
	{
		for (int j = 0; j < map->cols; j++)
		{
			if (map->grid[i][j] != map->empty && map->grid[i][j] != map->obstacle)
				return (1);
		}
	}
	return (0);
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

	t_map *map;
	map = calloc(1, sizeof(t_map));
	if (!map)
		return (1);

	int params = 0;
	params = fscanf(file, "%d %c %c %c", &map->num_rows, &map->empty, &map->obstacle, &map->full);
	if (params != 4)
	{

		fprintf(stderr, "map error\n");
		free_function(map, NULL);
		return (1);
	}
	char newline;
	params = fscanf(file, "%c", &newline);
	if (newline != '\n' || params != 1)
	{

		fprintf(stderr, "map error\n");
		free_function(map, NULL);
		return (1);
	}
	if (verify_header(map) == 1)
	{
		fprintf(stderr, "map error\n");
		free_function(map, NULL);
		return (1);
	}
	map->grid = calloc(map->num_rows, sizeof(char *));
	if (!map->grid)
		return (free(map), 1);
	for (int i = 0; i < map->num_rows; i++)
	{
		ssize_t byte_read = 0;
		size_t len = 0;
		byte_read = getline(&map->grid[i], &len, file);
		if (byte_read == -1)
		{
			fprintf(stderr, "map error\n");
			free_function(map, NULL);
			return (1);
		}
		if (i == 0)
			map->cols = byte_read - 1;
		else if (byte_read - 1 != map->cols)
		{
			fprintf(stderr, "map error\n");
			free_function(map, NULL);
			return (1);
		}

		if (map->grid[i][byte_read - 1] != '\n')
		{
			fprintf(stderr, "map error\n");
			free_function(map, NULL);
			return (1);
		}
	}
	char extra;

if (fscanf(file, "%c", &extra) == 1)
{
    fprintf(stderr, "map error\n");
    free_function(map, NULL);
    return (1);
}
	if (verify_map(map))
	{
		fprintf(stderr, "map error\n");
		free_function(map, NULL);
		return (1);
	}
	t_square *square = NULL;
	square = find_square(map);
	if (!square)
	{
		fprintf(stderr, "map error\n");
		free_function(map, square);
		return (1);
	}
	print_square(square, map);
	free_function(map, square);
	if (file != stdin)
		fclose(file);
	return (0);
}