#include <stdlib.h>
#include <stdio.h>


typedef struct Board
{
    int spaces[9][9];
    int options[9][9][10];
} Board;


int get_number_unknown_values(Board * board)
{
    int count = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (board->spaces[row][col] == 0)
            {
                count++;
            }
        }
    }
    return count;
}


int get_number_potential_values(Board * board, int row, int col)
{
    int count = 0;
    for (int option = 1; option < 10; option++)
    {
        if (board->options[row][col][option] == 0)
        {
            count++;
        }
    }
    return count;
}


int get_next_option(Board * board, int row, int col)
{
    for (int option = 1; option < 10; option++)
    {
        if (board->options[row][col][option] == 0)
        {
            return option;
        }
    }
    return -1;
}


void update_potentional_values(Board * board, int row, int col)
{
    // Eliminate taken values in the same row or same col
    for (int trow = 0; trow < 9; trow++)
    {
        int value = board->spaces[trow][col];
        board->options[row][col][value] = 1;
    }
    for (int tcol = 0; tcol < 9; tcol++)
    {
        int value = board->spaces[row][tcol];
        board->options[row][col][value] = 1;
    }

    // Eliminate taken values in the same box
    int rfactor = row / 3;
    int cfactor = col / 3;
    rfactor = rfactor * 3;
    cfactor = cfactor * 3;
    for (int trow = rfactor; trow < rfactor + 3; trow++)
    {
        for (int tcol = cfactor; tcol < cfactor + 3; tcol++)
        {
            int value = board->spaces[trow][tcol];
            board->options[row][col][value] = 1;
        }
    }

    // If check all options and if nothing in same row, same col, or
    // same box, set as actual value.
    int flag = 0;
    for (int option = 1; option < 10; option++)
    {
        if (board->options[row][col][option] == 0)
        {
            int count = 0;
            for (int trow = 0; trow < 9; trow++)
            {
                if (board->options[trow][col][option] == 0)
                {
                    count++;
                }
            }
            if (count == 1)
            {
                flag = option;
                break;
            }

            count = 0;
            for (int tcol = 0; tcol < 9; tcol++)
            {
                if (board->options[row][tcol][option] == 0)
                {
                    count++;
                }
            }
            if (count == 1)
            {
                flag = option;
                break;
            }

            count = 0;
            for (int trow = rfactor; trow < rfactor + 3; trow++)
            {
                for (int tcol = cfactor; tcol < cfactor + 3; tcol++)
                {
                    if (board->options[trow][tcol][option] == 0)
                    {
                        count++;
                    }
                }
            }
            if (count == 1)
            {
                flag = option;
                break;
            }
        }
    }

    if (flag > 0)
    {
        for (int option = 0; option < 10; option++)
        {
            board->options[row][col][option] = flag == option ? 0 : 1;
        }
    }
}


int update_known_values(Board * board)
{
    int fills = 0;
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            update_potentional_values(board, row, col);
            if (get_number_potential_values(board, row, col) == 1)
            {
                int option = get_next_option(board, row, col);
                board->spaces[row][col] = option;
                fills++;
                printf("Filling %d, %d: %d\n", row, col, option);
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

            // Reset potential values to 0 if value is unknown (0) or 1 if value is known
            board->options[row][col][0] = 1;
            for (int option = 1; option < 10; option++)
            {
                board->options[row][col][option] = value == 0 ? 0 : 1;
            }
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
    while ((fills = update_known_values(board)) > 0)
    {
        print_board(board);
    }

    int unsolved = get_number_unknown_values(board);
    if (unsolved == 0)
    {
        printf("Solved the puzzle!!\n");
    }

    free(board);
}
