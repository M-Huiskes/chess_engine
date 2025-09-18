#include <stdint.h>
#include <stdio.h>

const int FILE_OFFSET = 96;

// Rank 2 (white pawns) → bits 8–15
uint64_t WHITE_PAWNS = 0x000000000000FF00ULL;
// Rank 7 (black pawns) → bits 48–55
uint64_t BLACK_PAWNS = 0x00FF000000000000ULL;

// Rooks
uint64_t WHITE_ROOKS = 0x0000000000000081ULL;
uint64_t BLACK_ROOKS = 0x8100000000000000ULL;

// Knights
uint64_t WHITE_KNIGHTS = 0x0000000000000042ULL;
uint64_t BLACK_KNIGHTS = 0x4200000000000000ULL;

// Bishops
uint64_t WHITE_BISHOPS = 0x0000000000000024ULL;
uint64_t BLACK_BISHOPS = 0x2400000000000000ULL;

// Queens
uint64_t WHITE_QUEEN = 0x0000000000000008ULL;
uint64_t BLACK_QUEEN = 0x0800000000000000ULL;

// Kings
uint64_t WHITE_KING = 0x0000000000000010ULL;
uint64_t BLACK_KING = 0x1000000000000000ULL;

typedef struct
{
    uint64_t *bb;
    char symbol;
} PieceBB;

typedef struct 
{
    char file;
    int row;
} Square;


void print_bitboard(PieceBB pieces[])
{
    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            uint64_t mask = (uint64_t)1 << sq;
            char piece = '.';

            for (int i = 0; i < 12; i++)
            {
                if (*(pieces[i].bb) & mask)
                {
                    piece = pieces[i].symbol;
                }
            }
            printf("%c", piece);
        }
        printf("\n");
    }
    printf("  abcdefgh\n");
}

Square get_input_square() {
    char input[3];
    printf("Enter square (i.e. h1): ");
    if (fgets(input, sizeof(input), stdin)) {
        
    }
}


// PieceBB* find_piece_by_position(PieceBB pieces[], int position) {
//     uint64_t mask = ((int64_t)1 << )
// }


int main()
{
    PieceBB pieces[] = {
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
    print_bitboard(pieces);
    char c = 'a';
    printf("%d\n", c);
    return 0;
}