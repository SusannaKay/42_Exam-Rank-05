#include "life.h"

void ft_putstr(char *string){
    for(int i = 0; string[i] != '\0'; i++)
        putchar(string[i]);
    return;
}

int ft_strlen(char *string){
    int i = 0;
    while (string[i] != '\0')
        i++;
    return (i);
}

bool notDigit(char *string){
    for(int i= 0; string[i] != '\0'; i++)
    {
        if (string[i] < '0' || string[i] >'9')
            return true;
    }
    return false;
}

int main (int argc, char **argv){

    t_grid *game = NULL;
    if (argc != 4){
        ft_putstr("Error\n usage <./life widht height iteration>");
        return (1);
    }
    // game = malloc(1 * sizeof(t_grid));
    // if (!game)
    //     return(1);
    if(notDigit(argv[1]) || notDigit(argv[2]) || notDigit(argv[3]))
    {
        ft_putstr("Error\n use positive int only");
        return (1);
    }

    game->widht = atoi(argv[1]);
    if (game->widht == 0 && argv[1] != "0")
        return(free(game), 1);
    game->height = atoi(argv[2]);
        if (game->height == 0 && argv[2] != "0")
        return(free(game), 1);
    game->iterations = atoi(argv[3]);
        if (game->iterations == 0 && argv[3] != "0")
        return(free(game), 1);
    printf("%i, %i, %i", game->widht, game->height, game->iterations);
    game->board = malloc(game->height + 1 * sizeof(char *));
    if (!game->board)
        return (free(game), 1);
    game->board[game->height + 1] = NULL;
    for (int i = 0; game->board[i] != NULL; i++)
    {
        game->board[i] = malloc(game->widht + 1 * sizeof(char));
        if (!game->board[i])
            return (free(game), 1);
    }
      game->board = malloc(game->height + 1 * sizeof(char *));
    if (!game->board)
        return (free(game), 1);
    game->next[game->height + 1] = NULL;
    for (int i = 0; game->next[i] != NULL; i++)
    {
        game->next[i] = malloc(game->widht + 1 * sizeof(char));
        if (!game->next[i])
            return (free(game), 1);
    }


}