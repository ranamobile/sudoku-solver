#include <stdlib.h>
#include <stdio.h>


typedef struct Board
{
    int spaces[9][9];
} Board;


int * get_potential_values(Board * board, int row, int col)
{
    int * potential = (int *) malloc(sizeof(int) * 10);
    int value = board->spaces[row][col];
    for (int option = 0; option < 10; option++)
    {
        if (value == 0)
        {
            potential[option] = 0;
        }
        else
        {
            potential[option] = 1;
        }

    }

    potential[0] = 1;
    for (int trow = 0; trow < 9; trow++)
    {
        int value = board->spaces[trow][col];
        potential[value] = 1;
    }
    for (int tcol = 0; tcol < 9; tcol++)
    {
        int value = board->spaces[row][tcol];
        potential[value] = 1;
    }

    int rfactor = row / 3;
    int cfactor = col / 3;
    rfactor = rfactor * 3;
    cfactor = cfactor * 3;
    for (int trow = rfactor; trow < rfactor + 3; trow++)
    {
        for (int tcol = cfactor; tcol < cfactor + 3; tcol++)
        {
            int value = board->spaces[trow][tcol];
            potential[value] = 1;
        }
    }

    return potential;
}


int get_number_of_possibilities(int * possibilities)
{
    int count = 0;
    for (int option = 1; option < 10; option++)
    {
        if (possibilities[option] == 0)
        {
            count++;
        }
    }
    return count;
}


int get_next_option(int * possibilities, int index)
{
    int count = 0;
    for (int option = index; option < 10; option++)
    {
        if (possibilities[option] == 0)
        {
            return option;
        }
    }
    return -1;
}


int fill_known_values(Board * board)
{
    int * possibilities = NULL;
    int fills = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            possibilities = get_potential_values(board, row, col);
            if (get_number_of_possibilities(possibilities) == 1)
            {
                int option = get_next_option(possibilities, 0);
                board->spaces[row][col] = option;
                fills++;
                printf("Filling %d, %d: %d\n", row, col, option);
            }
            if (possibilities)
            {
                free(possibilities);
                possibilities = NULL;
            }
        }
    }
    return fills;
}


Board * load_board(char * filepath)
{
    FILE * fp = fopen(filepath, "r");
    ssize_t read;
    size_t len = 0;

    Board * board = (Board *) malloc(sizeof(Board));
    int value;

    if (fp == NULL)
    {
        return NULL;
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++)
        {
            fscanf(fp, "%i ", &value);
            board->spaces[row][col] = value;
        }
    }

    fclose(fp);
    return board;
}


void print_board(Board * board)
{
    printf("\n");
    for (int row = 0; row < 9; row++)
    {
        if (row > 0 && row % 3 == 0)
        {
            printf("-------------------------------\n");
        }
        for (int col = 0; col < 9; col++)
        {
            if (col > 0 && col % 3 == 0)
            {
                printf(" |");
            }
            printf(" %d ", board->spaces[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char * argv[])
{
    Board * board;
    if (argc != 2)
    {
        exit(EXIT_FAILURE);
    }

    printf("Filepath: %s\n", argv[1]);
    board = load_board(argv[1]);

    print_board(board);

    int fills;
    while ((fills = fill_known_values(board)) > 0)
    {
        print_board(board);
    }

    free(board);
}
