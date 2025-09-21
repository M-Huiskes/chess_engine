#ifndef PIECES_H
#define PIECES_H

#include <stdint.h>

typedef struct
{
    uint64_t *pos_bb;
    char symbol;
} Piece;

Piece *get_pieces(void);
uint64_t get_full_board(void);
// uint64_t calculate_possible_moves(Piece piece, uint64_t full_board, int position);
int calculate_possible_moves(int position);
#endif