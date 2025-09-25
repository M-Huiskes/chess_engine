#include <stdint.h>
#include <stdio.h>
#include <string.h>

// test 

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

Piece PIECES[12] = {
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

void print_bitboard(Square input_square, uint64_t possible_moves)
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
                    if (*(PIECES[i].pos_bb) & mask)
                    {
                        piece = PIECES[i].symbol;
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

uint64_t get_full_board()
{
    uint64_t board = (uint64_t)0;
    for (int i = 0; i < 12; i++)
    {
        board |= *(PIECES[i].pos_bb);
    }
    return board;
}

Piece *find_piece_by_position(int position)
{
    uint64_t mask = ((uint64_t)1 << position);
    for (int i = 0; i < 12; i++)
    {
        if (*(PIECES[i].pos_bb) & mask)
        {
            return &PIECES[i];
        }
    }
    Square square = square_from_position(position);
    char file_char = square.file + FILE_OFFSET;
    char row_char = square.row + ROW_OFFSET;
    return NULL;
}

int is_bit_set(uint64_t bb, int position)
{
    uint64_t mask = (uint64_t)1 << position;
    return (bb & mask) ? 1 : 0;
}

int is_enemy(Piece *piece, int position)
{
    Piece *other_piece = find_piece_by_position(position);

    if (piece->color != other_piece->color)
    {
        return 1;
    }
    return 0;
}

uint64_t find_possible_pawn_moves(Piece *piece, Square input_square, int position, uint64_t full_board)
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
            if (input_square.file < 7 && is_bit_set(full_board, (position + 9)) && is_enemy(piece, (position + 9)))
            {
                possible_moves |= ((uint64_t)1 << (position + 9));
            }
            if (input_square.file > 0 && is_bit_set(full_board, (position + 7)) && is_enemy(piece, (position + 7)))
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
            if (input_square.file < 7 && is_bit_set(full_board, (position - 7)) && is_enemy(piece, (position - 7)))
            {
                possible_moves |= ((uint64_t)1 << (position - 7));
            }
            if (input_square.file > 0 && is_bit_set(full_board, (position - 9)) && is_enemy(piece, (position - 9)))
            {
                possible_moves |= ((uint64_t)1 << (position - 9));
            }
        }
    }
    return possible_moves;
}

int check_diag_move(int position, int next_pos)
{
    if (next_pos < 0 || next_pos > 63)
    {
        return 0;
    }

    int next_row = next_pos / 8;
    int old_row = position / 8;
    int row_diff = next_row - old_row;
    if (row_diff == 1 || row_diff == -1)
    {
        return 1;
    }
    return 0;
}

void find_diagonal_moves(int position, uint64_t full_board, uint64_t *possible_moves, Piece *piece)
{
    int directions[4] = {7, 9, -7, -9};
    for (int i = 0; i < 4; i++)
    {
        printf("Direction: %d\n", directions[i]);
        int counter = 1;
        int next_pos = position + directions[i];
        int old_pos = position;
        printf("Check diag move: %d\n", check_diag_move(old_pos, next_pos));
        while (check_diag_move(old_pos, next_pos))
        {
            counter++;
            printf("Is bit set %d\n", is_bit_set(full_board, next_pos));
            printf("next pos: %d\n", next_pos);
            printf("Full bitboard:\n");
            print_single_bitboard(full_board);
            if (is_bit_set(full_board, next_pos))
            {
                if (is_enemy(piece, next_pos))
                {
                    *possible_moves |= (uint64_t)1 << next_pos;
                }
                break;
            }
            *possible_moves |= (uint64_t)1 << next_pos;
            old_pos = next_pos;
            next_pos = old_pos + directions[i];
        }
    }
}

uint64_t find_possible_bishop_moves(Piece *piece, int position, uint64_t full_board)
{
    printf("Entering bishop moves\n");
    uint64_t possible_moves = (uint64_t)0;
    find_diagonal_moves(position, full_board, &possible_moves, piece);
    print_single_bitboard(possible_moves);
    return possible_moves;
}

uint64_t find_possible_bishop_moves_2(Piece *piece, int position, uint64_t full_board)
{
    printf("Entering bishop moves\n");
    uint64_t possible_moves = (uint64_t)0;
    if (!(position % 8 == 0))
    {
        int counter = 1;
        // diagonal in direction a8
        int temp_position_a = position;
        while (temp_position_a % 8 != 0 && temp_position_a % 7 != 0)
        {
            temp_position_a = position + (7 * counter);
            if (temp_position_a < 0 || temp_position_a > 63)
            {
                break;
            }
            counter++;
            printf("Temp position a1 direction: %d\n", temp_position_a);
            if (is_bit_set(full_board, temp_position_a))
            {
                if (is_enemy(piece, temp_position_a))
                {
                    possible_moves |= (uint64_t)1 << temp_position_a;
                }
                break;
            }
            possible_moves |= (uint64_t)1 << temp_position_a;
        }
        counter = 1;
        // diagonal in direction a1
        temp_position_a = position;
        while (temp_position_a % 8 != 0 && temp_position_a % 7 != 0)
        {
            temp_position_a = position - (9 * counter);
            if (temp_position_a < 0 || temp_position_a > 63)
            {
                break;
            }
            printf("Temp position a1 direction: %d\n", temp_position_a);
            counter++;
            if (is_bit_set(full_board, temp_position_a))
            {
                if (is_enemy(piece, temp_position_a))
                {
                    possible_moves |= (uint64_t)1 << temp_position_a;
                }
                break;
            }
            possible_moves |= (uint64_t)1 << temp_position_a;
        }
    }

    if (!(position % 7 == 0))
    {
        int counter = 1;
        // diagonal in direction h8
        int temp_position_h = position;
        while (temp_position_h % 7 != 0 && temp_position_h % 8 != 0)
        {
            temp_position_h = position + (9 * counter);
            if (temp_position_h < 0 || temp_position_h > 63)
            {
                break;
            }
            counter++;
            if (is_bit_set(full_board, temp_position_h))
            {
                if (is_enemy(piece, temp_position_h))
                {
                    possible_moves |= (uint64_t)1 << temp_position_h;
                }
                break;
            }
            possible_moves |= (uint64_t)1 << temp_position_h;
        }

        counter = 1;
        // diagonal in direction h1
        temp_position_h = position;
        while (temp_position_h % 7 != 0 && temp_position_h % 8 != 0)
        {
            temp_position_h = position - (7 * counter);
            if (temp_position_h < 0 || temp_position_h > 63)
            {
                break;
            }
            counter++;
            if (is_bit_set(full_board, temp_position_h))
            {
                if (is_enemy(piece, temp_position_h))
                {
                    possible_moves |= (uint64_t)1 << temp_position_h;
                }
                break;
            }
            possible_moves |= (uint64_t)1 << temp_position_h;
        }
    }
    return possible_moves;
}

uint64_t find_possible_moves(Square input_square)
{
    uint64_t full_board = get_full_board();
    int position = get_position(input_square);
    printf("Position of selected piece: %d\n", position);
    Piece *piece = find_piece_by_position(position);

    switch (piece->symbol)
    {
    case 'P':
    case 'p':
        return find_possible_pawn_moves(piece, input_square, position, full_board);
    case 'B':
    case 'b':
        return find_possible_bishop_moves(piece, position, full_board);
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
    return is_bit_set(possible_moves, position);
}

void make_move(Square input_square, Square output_square)
{
    int old_pos = get_position(input_square);
    Piece *piece = find_piece_by_position(old_pos);
    int new_pos = get_position(output_square);

    Piece *other_piece = find_piece_by_position(new_pos);

    if (!(other_piece == NULL))
    {
        unset_bit(other_piece->pos_bb, new_pos);
    }

    unset_bit(piece->pos_bb, old_pos);
    set_bit(piece->pos_bb, new_pos);
}

int main()
{
    Square init_square = {-1, -1};
    print_bitboard(init_square, (uint64_t)0);
    int loop = 1;
    while (loop)
    {
        Square input_square = get_input_square();
        uint64_t possible_moves = find_possible_moves(input_square);
        print_bitboard(input_square, possible_moves);

        Square output_square = get_input_square();
        while (!is_move_possible(output_square, possible_moves))
        {
            printf("move is not possible\n");
            output_square = get_input_square();
        }
        make_move(input_square, output_square);
        print_bitboard(init_square, (uint64_t)0);
    }
    return 0;
}