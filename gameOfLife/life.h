#ifndef LIFE_H
# define LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//atoi, read, putchar, malloc, calloc, realloc, free

typedef struct s_grid{

    int widht;
    int height;
    int iterations;
    char **board;
    char **next;
} t_grid;



#endif