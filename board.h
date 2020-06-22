#ifndef BOARD_H
#define BOARD_H

const int MAX_NUM_GUESSES = 2;

typedef struct Board
{
    int spaces[9][9];
    int options[9][9][10];
} Board;

int get_number_unknown_values(Board * board);

int get_number_potential_values(Board * board, int row, int col);

int get_next_option(Board * board, int row, int col);

void update_potentional_values(Board * board, int row, int col);

int update_known_values(Board * board);

Board * load_board(char * filepath);

void print_board(Board * board);

#endif
