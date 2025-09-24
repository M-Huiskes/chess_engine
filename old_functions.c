#include <stdint.h>
#include <stdio.h>
#include <string.h>

const int SQUARE_SIZE = 75;
const int FILE_OFFSET = 'a';
const int ROW_OFFSET = '1';

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

typedef struct
{
    uint64_t *pos_bb;
    char symbol;
    char color;
} Piece;

typedef struct
{
    int file;
    int row;
} Square;

void print_single_bitboard(uint64_t bb)
{
    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            int square = row * 8 + col;
            uint64_t mask = (uint64_t)1 << square;
            printf("%d ", (bb & mask) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n");
}

void print_bitboard(Piece pieces[], Square input_square, uint64_t possible_moves)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            uint64_t mask = (uint64_t)1 << sq;
            char piece = '.';

            if (possible_moves & mask)
            {
                piece = 'x';
            }
            else
            {
                for (int i = 0; i < 12; i++)
                {
                    if (*(pieces[i].pos_bb) & mask)
                    {
                        piece = pieces[i].symbol;
                    }
                }
            }
            if (rank == input_square.row && file == input_square.file)
            {
                printf("x");
            }
            else
            {
                printf("%c", piece);
            }
        }
        printf("\n");
    }
    printf("  abcdefgh\n");
}

int get_position(Square square)
{
    return square.row * 8 + square.file;
}

Square square_from_position(int position)
{
    int file = position % 8;
    int row = (position - file) / 8;
    return (Square){file, row};
}

uint64_t get_full_board(Piece pieces[])
{
    uint64_t board = (uint64_t)0;
    for (int i = 0; i < 12; i++)
    {
        board |= *(pieces[i].pos_bb);
    }
    return board;
}

Piece *find_piece_by_position(Piece pieces[], int position)
{
    uint64_t mask = ((uint64_t)1 << position);
    for (int i = 0; i < 12; i++)
    {
        if (*(pieces[i].pos_bb) & mask)
        {
            return &pieces[i];
        }
    }
    Square square = square_from_position(position);
    char file_char = square.file + FILE_OFFSET;
    char row_char = square.row + ROW_OFFSET;
    printf("Did not find piece on square %c%c\n", file_char, row_char);
    return NULL;
}

int is_bit_set(uint64_t bb, int position)
{
    uint64_t mask = (uint64_t)1 << position;
    return (bb & mask) ? 1 : 0;
}

int is_enemy(Piece *piece, int position, Piece pieces[])
{
    Piece *other_piece = find_piece_by_position(pieces, position);
}

uint64_t find_possible_pawn_moves(Piece pieces[], Piece *piece, Square input_square, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;

    if (piece->symbol == 'P')
    {
        if (input_square.row < 8)
        {
            possible_moves |= ((uint64_t)1 << (position + 8));
            if (input_square.row == 1)
            {
                possible_moves |= ((uint64_t)1 << (position + 16));
            }
            if (input_square.file < 7 && is_bit_set(full_board, (position + 9)))
            {
                possible_moves |= ((uint64_t)1 << (position + 9));
            }
            if (input_square.file > 0 && is_bit_set(full_board, (position + 7)))
            {
                possible_moves |= ((uint64_t)1 << (position + 7));
            }
        }
    }
    else
    {
        if (input_square.row > 0)
        {
            possible_moves |= ((uint64_t)1 << (position - 8));
            if (input_square.row == 6)
            {
                possible_moves |= ((uint64_t)1 << (position - 16));
            }
            if (input_square.file < 7 && is_bit_set(full_board, (position - 7)))
            {
                possible_moves |= ((uint64_t)1 << (position - 7));
            }
            if (input_square.file > 0 && is_bit_set(full_board, (position - 9)))
            {
                possible_moves |= ((uint64_t)1 << (position - 9));
            }
        }
    }
    return possible_moves;
}

uint64_t find_possible_bishop_moves(Piece pieces[], Piece *piece, Square input_square, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;
    printf("initial position: %d\n", position);
    if (!(position % 8 == 0))
    {
        int counter = 1;
        // diagonal in direction a8
        int temp_position_a = position + 7;
        while (temp_position_a % 8 != 0 && temp_position_a <= 63)
        {
            temp_position_a = position + (7 * counter);
            counter++;
            possible_moves |= (uint64_t)1 << temp_position_a;
        }
        counter = 1;
        // diagonal in direction a1
        temp_position_a = position - 9;
        while (temp_position_a % 8 != 0 && temp_position_a >= 0)
        {
            temp_position_a = position - (9 * counter);
            counter++;
            possible_moves |= (uint64_t)1 << temp_position_a;
        }
    }

    if (!(position % 7 == 0))
    {
        int counter = 1;
        // diagonal in direction h8
        int temp_position_h = position + 9;
        while (temp_position_h % 7 != 0 && temp_position_h <= 63)
        {
            temp_position_h = position + (9 * counter);
            counter++;
            possible_moves |= (uint64_t)1 << temp_position_h;
        }

        counter = 1;
        // diagonal in direction h1
        temp_position_h = position - 7;
        while (temp_position_h % 7 != 0 && temp_position_h >= 0)
        {
            temp_position_h = position - (7 * counter);
            counter++;
            possible_moves |= (uint64_t)1 << temp_position_h;
        }
    }
    return possible_moves;
}

uint64_t find_possible_moves(Square input_square, Piece pieces[])
{
    uint64_t full_board = get_full_board(pieces);
    int position = get_position(input_square);
    printf("Position of selected piece: %d", position);
    Piece *piece = find_piece_by_position(pieces, position);

    switch (piece->symbol)
    {
    case 'P':
    case 'p':
        return find_possible_pawn_moves(pieces, piece, input_square, position, full_board);
    case 'B':
    case 'b':
        return find_possible_bishop_moves(pieces, piece, input_square, position, full_board);
    }
}

void set_bit(uint64_t *piece_bb, int position)
{
    uint64_t mask = (uint64_t)1 << position;
    *piece_bb |= mask;
}

void unset_bit(uint64_t *piece_bb, int position)
{
    uint64_t mask = (uint64_t)1 << position;
    *piece_bb &= ~mask;
}

Square get_input_square(void)
{
    char input[100];

    while (1)
    {
        printf("Enter square (i.e. h1): ");

        if (!fgets(input, sizeof(input), stdin))
        {
            Square invalid = {-1, -1};
            return invalid;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) != 2 || input[0] > 'h' || input[1] > '8')
        {
            printf("Invalid input, please enter like h1.\n");
            continue;
        }

        char file_char = input[0];
        char row_char = input[1];

        int file = file_char - FILE_OFFSET;
        int row = row_char - ROW_OFFSET;

        Square square = {file, row};
        return square;
    }
}

int is_move_possible(Square output_square, uint64_t possible_moves)
{
    int position = get_position(output_square);
    printf("Position: %d\n", position);
    print_single_bitboard(possible_moves);
    printf("is bit set: %d\n", is_bit_set(possible_moves, position));
    return is_bit_set(possible_moves, position);
}

void make_move(Square input_square, Square output_square, Piece pieces[])
{
    int old_pos = get_position(input_square);
    Piece *piece = find_piece_by_position(pieces, old_pos);
    int new_pos = get_position(output_square);

    Piece *other_piece = find_piece_by_position(pieces, new_pos);

    if (!(other_piece == NULL))
    {
        unset_bit(other_piece->pos_bb, new_pos);
    }

    unset_bit(piece->pos_bb, old_pos);
    set_bit(piece->pos_bb, new_pos);
}

int main()
{
    Piece pieces[12] = {
        {&WHITE_PAWNS, 'P', 'w'},
        {&BLACK_PAWNS, 'p', 'b'},
        {&WHITE_ROOKS, 'R', 'w'},
        {&BLACK_ROOKS, 'r', 'b'},
        {&WHITE_KNIGHTS, 'N', 'w'},
        {&BLACK_KNIGHTS, 'n', 'b'},
        {&WHITE_BISHOPS, 'B', 'w'},
        {&BLACK_BISHOPS, 'b', 'b'},
        {&WHITE_QUEEN, 'Q', 'w'},
        {&BLACK_QUEEN, 'q', 'b'},
        {&WHITE_KING, 'K', 'w'},
        {&BLACK_KING, 'k', 'b'},
    };
    Square init_square = {-1, -1};
    print_bitboard(pieces, init_square, (uint64_t)0);
    int loop = 1;
    while (loop)
    {
        Square input_square = get_input_square();
        uint64_t possible_moves = find_possible_moves(input_square, pieces);
        print_bitboard(pieces, input_square, possible_moves);

        Square output_square = get_input_square();
        while (!is_move_possible(output_square, possible_moves))
        {
            printf("move is not possible\n");
            output_square = get_input_square();
        }
        make_move(input_square, output_square, pieces);
        print_bitboard(pieces, init_square, (uint64_t)0);
    }
    return 0;
}