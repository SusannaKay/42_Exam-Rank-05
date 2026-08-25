#ifndef BSQ_H
# define BSQ_H

#include <stdio.h>
#include <stdlib.h>

typedef struct s_map{

	int num_rows;
	int cols;
	char full;
	char empty;
	char obstacle;
	char **grid;

} t_map;

typedef struct s_square{

	int size;
	int row;
	int col;
} t_square;

#endif