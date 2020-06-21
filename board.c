#include <stdlib.h>
#include <stdio.h>


typedef struct Board
{
    int spaces[9][9];
} Board;


int * get_potential_values(Board * board, int row, int col)
{
    int * potential = (int *) malloc(sizeof(int) * 9);
    for (int option = 0; option < 9; option++)
    {
        potential[option] = 1;
    }
    for (int trow = 0; trow < 9; trow++)
    {
        int value = board->spaces[trow][col];
        potential[value] = 1;
    }
    for (int tcol = 0; tcol < 9; tcol++)
    {
        int value = board->spaces[row][tcol];
        potential[value] = 0;
    }

    return potential;
}

Board * load_board(char * filepath)
{

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
            printf(" %d ", board->spaces[row][col] + 1);
        }
        printf("\n");
    }
    printf("\n");
}


int main()
{
    Board * board = (Board *) malloc(sizeof(Board));
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            board->spaces[row][col] = 0;
        }
    }

    print_board(board);

    int * possibilities = get_potential_values(board, 0, 0);
    for (int option = 0; option < 9; option++)
    {
        if (possibilities[option] > 0)
        {
            printf("%d ", option + 1);
        }
    }
    printf("\n");
}
