#include <stdint.h>
#include <stdio.h>
#include "pieces.h"

uint64_t WHITE_PAWNS = 0x000000000000FF00ULL;
uint64_t BLACK_PAWNS = 0x00FF000000000000ULL;

uint64_t WHITE_ROOKS = 0x0000000000000081ULL;
uint64_t BLACK_ROOKS = 0x8100000000000000ULL;

uint64_t WHITE_KNIGHTS = 0x0000000000000042ULL;
uint64_t BLACK_KNIGHTS = 0x4200000000000000ULL;

uint64_t WHITE_BISHOPS = 0x0000000000000024ULL;
uint64_t BLACK_BISHOPS = 0x2400000000000000ULL;

uint64_t WHITE_QUEEN = 0x0000000000000008ULL;
uint64_t BLACK_QUEEN = 0x0800000000000000ULL;

uint64_t WHITE_KING = 0x0000000000000010ULL;
uint64_t BLACK_KING = 0x1000000000000000ULL;

Piece pieces[12] = {
    {&WHITE_PAWNS, 'P'},
    {&BLACK_PAWNS, 'p'},
    {&WHITE_ROOKS, 'R'},
    {&BLACK_ROOKS, 'r'},
    {&WHITE_KNIGHTS, 'N'},
    {&BLACK_KNIGHTS, 'n'},
    {&WHITE_BISHOPS, 'B'},
    {&BLACK_BISHOPS, 'b'},
    {&WHITE_QUEEN, 'Q'},
    {&BLACK_QUEEN, 'q'},
    {&WHITE_KING, 'K'},
    {&BLACK_KING, 'k'},
};

uint64_t get_full_board(void)
{
    uint64_t board = (uint64_t)0;
    for (int i = 0; i < 12; i++)
    {
        board |= *(pieces[i].pos_bb);
    }
    return board;
}

uint64_t white_pawn_moves(uint64_t full_board, int position)
{
    uint64_t possible_moves = (uint64_t)1 << (position + 8);
    return possible_moves;
}

// uint64_t calculate_possible_moves(Piece piece, uint64_t full_board, int position)
// {
//     switch (piece.symbol)
//     {
//     case 'P':
//         return white_pawn_moves(full_board, position);
//     default:
//         return NULL;
//     }
// }

int calculate_possible_moves(int position)
{
    return position + 8;
}

Piece *get_pieces(void)
{
    return pieces;
}