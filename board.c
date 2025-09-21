#include <stdint.h>
#include <stdio.h>
#include <string.h>

const int FILE_OFFSET = 'a';
const int ROW_OFFSET = '1';

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
    int file;
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

int get_position(int file, int row)
{
    return row * 8 + file;
}

Square square_from_position(int position)
{
    int file = position % 8;
    int row = (position - file) / 8;
    return (Square){file, row};
}

Square get_input_square(void)
{
    char input[100]; // big enough to hold the whole line

    while (1)
    {
        printf("Enter square (i.e. h1): ");

        if (!fgets(input, sizeof(input), stdin))
        {
            // Input error or EOF
            Square invalid = {-1, -1};
            return invalid;
        }

        // Remove trailing newline if it exists
        input[strcspn(input, "\n")] = '\0';

        // Check length (must be exactly 2: e.g. 'h' and '1')
        if (strlen(input) != 2 || input[0] > 'h' || input[1] > '8')
        {
            printf("Invalid input, please enter like h1.\n");
            continue; // ask again
        }

        char file_char = input[0];
        char row_char = input[1];

        int file = file_char - FILE_OFFSET;
        int row = row_char - ROW_OFFSET;

        Square square = {file, row};
        return square;
    }
}


void set_bit(uint64_t *piece_bb, int position) {
    uint64_t mask = (uint64_t)1 << position;
    *piece_bb |= mask;
}

void unset_bit(uint64_t *piece_bb, int position) {
    uint64_t mask = (uint64_t)1 << position;
    *piece_bb &= ~mask;
}

// Deze functie generiek maken -> zie print_bitboard()
PieceBB *find_piece_by_position(PieceBB pieces[], int position)
{
    uint64_t mask = ((int64_t)1 << position);
    int found_piece = 0;
    for (int i = 0; i < 12; i++)
    {
        if (*(pieces[i].bb) & mask)
        {
            found_piece = 1;
            return &pieces[i];
        }
    }
    if (!found_piece)
    {
        Square square = square_from_position(position);
        char file_char = square.file + FILE_OFFSET;
        char row_char = square.row + ROW_OFFSET;
        printf("Did not find piece on square %c%c\n", file_char, row_char);
        return NULL;
    }
}

void make_move(Square input, Square output, PieceBB pieces[])
{
    int input_pos = get_position(input.file, input.row);
    int output_pos = get_position(output.file, output.row);

    // Hier nog checken of er geen null pointer terugkomt
    PieceBB *piece = find_piece_by_position(pieces, input_pos);
    unset_bit(piece->bb, input_pos);
    set_bit(piece->bb, output_pos);
}

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

    int moves = 0;
    while (moves < 5)
    {
        moves++;
        Square input_square = get_input_square();
        Square output_square = get_input_square();
        make_move(input_square, output_square, pieces);
        print_bitboard(pieces);
    }

    return 0;
}