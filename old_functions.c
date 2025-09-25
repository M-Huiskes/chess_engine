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
            if (!(is_bit_set(full_board, (position + 8))))
            {
                possible_moves |= ((uint64_t)1 << (position + 8));
            }
            if (input_square.row == 1)
            {
                if (!(is_bit_set(full_board, (position + 16))))
                {
                    possible_moves |= ((uint64_t)1 << (position + 16));
                }
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
            if (!(is_bit_set(full_board, (position - 8))))
            {
                possible_moves |= ((uint64_t)1 << (position - 8));
            }
            if (input_square.row == 6)
            {
                if (!(is_bit_set(full_board, (position - 16))))
                {
                    possible_moves |= ((uint64_t)1 << (position - 16));
                }
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
        int counter = 1;
        int next_pos = position + directions[i];
        int old_pos = position;
        while (check_diag_move(old_pos, next_pos))
        {
            counter++;
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

int check_vertical_move(int next_pos)
{
    return (next_pos < 0 || next_pos > 63) ? 0 : 1;
}

int check_horizontal_move(int position, int next_pos)
{
    if (next_pos < 0 || next_pos > 63)
    {
        return 0;
    }
    int next_row = next_pos / 8;
    int old_row = position / 8;
    int row_diff = next_row - old_row;

    if (row_diff == 0)
    {
        return 1;
    }
    return 0;
}

void find_orthogonal_moves(int position, uint64_t full_board, uint64_t *possible_moves, Piece *piece)
{
    int hor_dir[2] = {-1, 1};
    for (int i = 0; i < 2; i++)
    {
        int counter = 1;
        int next_pos = position + hor_dir[i];
        int old_pos = position;
        while (check_horizontal_move(old_pos, next_pos))
        {
            counter++;
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
            next_pos = old_pos + hor_dir[i];
        }
    }

    int ver_dir[2] = {-8, 8};
    for (int i = 0; i < 2; i++)
    {
        int counter = 1;
        int next_pos = position + ver_dir[i];
        int old_pos = position;
        while (check_vertical_move(next_pos))
        {
            counter++;
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
            next_pos = old_pos + ver_dir[i];
        }
    }
}

uint64_t find_possible_bishop_moves(Piece *piece, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;
    find_diagonal_moves(position, full_board, &possible_moves, piece);
    return possible_moves;
}

uint64_t find_possible_queen_moves(Piece *piece, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;
    find_diagonal_moves(position, full_board, &possible_moves, piece);
    find_orthogonal_moves(position, full_board, &possible_moves, piece);
    return possible_moves;
}

uint64_t find_possible_rook_moves(Piece *piece, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;
    find_orthogonal_moves(position, full_board, &possible_moves, piece);
    return possible_moves;
}

uint64_t find_possible_knight_moves(Piece *piece, int position, uint64_t full_board)
{
    uint64_t possible_moves = (uint64_t)0;
    int ver_dir[2] = {-8, 8};
    int hor_step[2] = {-1, 1};
    for (int i = 0; i < 2; i++)
    {
        int pos_1 = position + 2 * ver_dir[i];
        if (check_vertical_move(pos_1))
        {
            for (int j = 0; j < 2; j++)
            {
                int new_pos = pos_1 + hor_step[j];
                if (check_horizontal_move(pos_1, new_pos))
                {
                    possible_moves |= (uint64_t)1 << new_pos;
                }
            }
        }
        int pos_2 = position + 2 * hor_step[i];
        if (check_horizontal_move(position, pos_2))
        {
            for (int j = 0; j < 2; j++)
            {
                int new_hor = pos_2 + ver_dir[j];
                if (check_vertical_move(new_hor))
                {
                    if (is_bit_set(full_board, position) && !(is_enemy(piece, position)))
                    {
                        continue;
                    }
                    possible_moves |= (uint64_t)1 << new_hor;
                }
            }
        }
    }
    return possible_moves;
}

uint64_t find_possible_moves(Square input_square)
{
    uint64_t full_board = get_full_board();
    int position = get_position(input_square);
    Piece *piece = find_piece_by_position(position);

    switch (piece->symbol)
    {
    case 'P':
    case 'p':
        return find_possible_pawn_moves(piece, input_square, position, full_board);
    case 'B':
    case 'b':
        return find_possible_bishop_moves(piece, position, full_board);
    case 'R':
    case 'r':
        return find_possible_rook_moves(piece, position, full_board);
    case 'Q':
    case 'q':
        return find_possible_queen_moves(piece, position, full_board);
    case 'N':
    case 'n':
        return find_possible_knight_moves(piece, position, full_board);
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

Square get_input_square(int is_init)
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

        int position = get_position(square);
        uint64_t full_board = get_full_board();

        Piece *piece = find_piece_by_position(position);

        if (piece == NULL && is_init) {
            printf("No piece found on square, try again");
            continue;
        }

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
        Square input_square = get_input_square(1);
        uint64_t possible_moves = find_possible_moves(input_square);
        print_bitboard(input_square, possible_moves);

        Square output_square = get_input_square(0);
        while (!is_move_possible(output_square, possible_moves))
        {
            printf("move is not possible\n");
            output_square = get_input_square(0);
        }
        make_move(input_square, output_square);
        print_bitboard(init_square, (uint64_t)0);
    }
    return 0;
}