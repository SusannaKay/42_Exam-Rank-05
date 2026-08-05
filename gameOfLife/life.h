#ifndef LIFE_H
# define LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
     #include <sys/types.h>
     #include <sys/uio.h>
     #include <unistd.h>
//atoi, read, putchar, malloc, calloc, realloc, free

typedef struct s_grid{

    int widht;
    int height;
    int iterations;
    char **board;
    char **next;
    int p_x;
    int p_y;
} t_grid;



#endif