#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void print_bitboard(uint64_t board)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            uint64_t mask = (uint64_t)1 << sq;
            printf("%c", (board & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("  abcdefgh\n");
}

void set_bit(uint64_t *bit_board, int position)
{
    // All bits will remain the same, only the bit where position is 1 will be set for sure (inclusive OR)
    *bit_board |= ((uint64_t)1 << position);
}

void set_initial_values(uint64_t *bit_board, int positions[], int length, bool is_white)
{
    for (int i = 0; i < length; i++)
    {
        int position = positions[i];
        if (!is_white)
        {
            position = 64 - position - 1;
        }
        set_bit(bit_board, position);
    }
}

int main()
{
    int pawn_positions[8] = {8, 9, 10, 11, 12, 13, 14, 15};
    int rook_positions[2] = {0, 7};
    int knight_positions[2] = {1, 6};
    int bishop_positions[2] = {2, 5};
    int queen_position[1] = {3};
    int king_position[1] = {4};

    uint64_t white_pawns = (uint64_t)0;
    uint64_t black_pawns = (uint64_t)0;

    uint64_t white_rook = (uint64_t)0;
    uint64_t black_rook = (uint64_t)0;

    uint64_t white_knight = (uint64_t)0;
    uint64_t black_knight = (uint64_t)0;

    uint64_t white_bishop = (uint64_t)0;
    uint64_t black_bishop = (uint64_t)0;

    uint64_t white_queen = (uint64_t)0;
    uint64_t black_queen = (uint64_t)0;

    uint64_t white_king = (uint64_t)0;
    uint64_t black_king = (uint64_t)0;

    set_initial_values(&white_pawns, pawn_positions, 8, true);
    set_initial_values(&black_pawns, pawn_positions, 8, false);

    set_initial_values(&white_rook, rook_positions, 2, true);
    set_initial_values(&black_rook, rook_positions, 2, false);

    set_initial_values(&white_knight, knight_positions, 2, true);
    set_initial_values(&black_knight, knight_positions, 2, false);

    set_initial_values(&white_bishop, bishop_positions, 2, true);
    set_initial_values(&black_bishop, bishop_positions, 2, false);

    set_initial_values(&white_queen, queen_position, 1, true);
    set_initial_values(&black_queen, queen_position, 1, false);

    set_initial_values(&white_king, king_position, 1, true);
    set_initial_values(&black_king, king_position, 1, false);

    print_bitboard(white_pawns | black_pawns | white_rook | black_rook | white_knight | black_knight | white_bishop | black_bishop | white_queen | black_queen | white_king | black_king);
    return 0;
}