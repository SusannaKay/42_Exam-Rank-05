#include "life.h"

void freeStruct(t_grid *game){
	if(game->board)
	{
		for (int i = 0; i < game->height; i++)
		{
			free(game->board[i]);
		}
		free(game->board);
		game->board = NULL;
	}
	if(game->next)
	{
		for (int i= 0; i < game->height; i++)
			free(game->next[i]);
		free(game->next);
		game->next = NULL;
	}
	free(game);
}

bool notDigit(char *string){
	for(int i = 0; string[i] != '\0'; i++)
	{
		if(string[i] < '0' || string[i] > '9')
			return true;
	}
	return false;
}

int init_matrix(t_grid *game)
{

	game->board = malloc((game->height + 1) * sizeof(char *));
	if (!game->board)
		return 1;
	game->board[game->height] = NULL;
	game->next = malloc((game->height + 1) * sizeof(char*));
	if(!game->next)
		return 1;
	game->next[game->height] = NULL;
	for (int i = 0; i < game->height ; i++)
	{
		game->board[i] = malloc((game->widht + 1) * sizeof(char));
		if (!game->board[i])
			return(1);
		game->next[i] = malloc((game->widht + 1) * sizeof(char));
		if(!game->next[i])
			return(1);
		game->board[i][game->widht] = '\0';
		game->next[i][game->widht] = '\0';
	}

	return (0);
}

void clear_matrix(char **matrix, int widht, int heigth){
	for(int i = 0;i < heigth; i++)
	{
		for(int j = 0; j < widht ; j++)
			matrix[i][j] = ' ';
	}
}

int init_game(t_grid *game, char **argv)
{
	game->widht = atoi(argv[1]);
	game->height = atoi(argv[2]);
	game->iterations = atoi(argv[3]);
	if (init_matrix(game) == 1)
		return (1);
	clear_matrix(game->board, game->widht, game->height);
	clear_matrix(game->next, game->widht, game->height);
	game->p_x = 0;
	game->p_y = 0;
	return (0);
}

void fillBoard(t_grid *game){
	char buffer;
	bool pen = false;
	while(read(STDIN_FILENO, &buffer, 1) > 0)
	{
		switch (buffer)
		{
			case 'w':
				if (game->p_y != 0)
					game->p_y--;
				break;
			case 's':
				if(game->p_y < game->height - 1)
					game->p_y++;
				break;
			case 'a':
				if(game->p_x != 0)
					game->p_x--;
				break;
			case 'd':
				if(game->p_x < game->widht - 1)
					game->p_x++;
				break;
			case 'x':
				pen = !pen;
				break;
			default:
				break;
		}
		if (pen)
			game->board[game->p_y][game->p_x] = 'O';
	}
}
void printBoard(char **matrix){
	putchar('\n');
	for(int i = 0; matrix[i] != NULL ; i++)
	{
		for (int j = 0; matrix[i][j] != '\0'; j++)
			putchar(matrix[i][j]);
		putchar('\n');
	}
}

void copyBoard(t_grid *game){
	for(int i = 0; i < game->height; i++)
	{
		for (int j = 0; j < game->widht; j++)
			game->board[i][j] = game->next[i][j];
	}
}

int countAlive(char **matrix, int i, int j, int width, int height){

	int count = 0;
	if (i != 0 && matrix[i - 1][j] == 'O')
		count++;
	if (j != 0 && matrix[i][j -1] == 'O')
		count++;
	if (i < (height - 1) && matrix[i + 1][j] == 'O')
		count++;
	if (j < (width - 1) && matrix[i][j + 1] == 'O')
		count++;
	if (i < (height - 1) && j < (width - 1) && matrix[i + 1][j + 1] == 'O')
		count++;
	if (i > 0 && j > 0 && matrix[i - 1][j - 1] == 'O')
		count++;
	if (i > 0 && j < (width - 1) && matrix[i - 1][j + 1] == 'O')
		count++;
	if ( i < (height - 1) && j > 0 && matrix[i + 1] [j - 1] == 'O')
		count++;
	return(count);
}

void play(t_grid *game)
{
	int count = 0;
	int i = 0;
	int j = 0;
	while (i < game->height)
	{
		count = 0;
		j= 0;
		while (j < game->widht)
		{
			count = countAlive(game->board, i, j, game->widht, game->height);
			if (game->board[i][j] == 'O' && (count == 2 || count == 3))
				game->next[i][j] = 'O';
			else if (game->board[i][j] == ' ' && count == 3)
				game->next[i][j] = 'O';
			else
				game->next[i][j] = ' ';
			j++;
		}
		i++;
	}
	copyBoard(game);
	clear_matrix(game->next, game->widht, game->height);
}

int main(int argc, char **argv){
	if (argc != 4)
		return (1);
	t_grid *game;
	game = malloc(sizeof(t_grid));
	if(!game)
		return(1);
	if(notDigit(argv[1]) || notDigit(argv[2]) || notDigit(argv[3]))
		return (1);
	if (init_game(game, argv) == 1)
		return(freeStruct(game), 1);
	fillBoard(game);
	while(game->iterations > 0)
	{
		play(game);
		game->iterations--;
	}
	printBoard(game->board);
	return(freeStruct(game), 0);
}