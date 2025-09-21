#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pieces.h"

const int SQUARE_SIZE = 75;
const int FILE_OFFSET = 'a';
const int ROW_OFFSET = '1';

typedef struct
{
    int file;
    int row;
} Square;

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

// Deze functie generiek maken -> zie print_bitboard()
Piece *find_piece_by_position(Piece pieces[], int position)
{
    uint64_t mask = ((int64_t)1 << position);
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

void make_move(Square input, Square output, Piece pieces[])
{
    int input_pos = get_position(input.file, input.row);
    int output_pos = get_position(output.file, output.row);

    // Hier nog checken of er geen null pointer terugkomt
    Piece *piece = find_piece_by_position(pieces, input_pos);
    unset_bit(piece->pos_bb, input_pos);
    set_bit(piece->pos_bb, output_pos);
}

const char *get_image_path(char symbol)
{
    switch (symbol)
    {
    case 'P':
        return "images/pawn_w.png";
    case 'p':
        return "images/pawn_b.png";
    case 'R':
        return "images/rook_w.png";
    case 'r':
        return "images/rook_b.png";
    case 'N':
        return "images/knight_w.png";
    case 'n':
        return "images/knight_b.png";
    case 'B':
        return "images/bishop_w.png";
    case 'b':
        return "images/bishop_b.png";
    case 'Q':
        return "images/queen_w.png";
    case 'q':
        return "images/queen_b.png";
    case 'K':
        return "images/king_w.png";
    case 'k':
        return "images/king_b.png";
    default:
        return NULL;
    }
}

void draw_possible_moves(SDL_Renderer *renderer, char board[8][8], int square_size, uint64_t pos_mov)
{
    for (int row = 7; row >= 0; row--)
    {
        for (int file = 0; file < 8; file++)
        {

            int sq = row * 8 + file;
            uint64_t mask = (uint64_t)1 << sq;
            char piece = 0;

            if (pos_mov & mask)
            {
                SDL_Rect rect = {row * square_size, file * square_size, square_size, square_size};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void render_board(SDL_Renderer *renderer, char board[8][8], Piece pieces[], int square_size, int sel_file, int sel_row)
{
    for (int row = 0; row < 8; row++)
    {
        for (int file = 0; file < 8; file++)
        {
            int y = 7 - row;
            int x = file;

            SDL_Rect rect = {x * square_size, y * square_size, square_size, square_size};
            if (sel_file == file && sel_row == row)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                uint64_t full_board = get_full_board();
                int position = get_position(file, row);
                Piece *piece = find_piece_by_position(pieces, position);
                // uint64_t pos_mov = calculate_possible_moves(*piece, full_board, position);
                int pos_mov = calculate_possible_moves(position);
                // draw_possible_moves(renderer, board, square_size, pos_mov);
                Square square = square_from_position(pos_mov);
                SDL_Rect rect_2 = {square.file * square_size, square.row * square_size, square_size, square_size};
                SDL_RenderFillRect(renderer, &rect);
            }
            else if ((x + row) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
            else
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
            SDL_RenderFillRect(renderer, &rect);

            if (board[row][file] != 0)
            {
                SDL_Texture *tex = NULL;
                for (int i = 0; i < 12; i++)
                {
                    char symbol = pieces[i].symbol;
                    if (symbol == board[row][file])
                    {
                        SDL_Surface *surface = IMG_Load(get_image_path(symbol));
                        if (!surface)
                        {
                            printf("Failed to load image: %s\n", IMG_GetError());
                            continue;
                        }
                        tex = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_FreeSurface(surface);
                        break;
                    }
                }
                if (tex)
                {
                    SDL_Rect pieceRect = {
                        x * square_size,
                        y * square_size,
                        square_size,
                        square_size};
                    SDL_RenderCopy(renderer, tex, NULL, &pieceRect);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void bitboards_to_board(Piece pieces[], char board[8][8])
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            uint64_t mask = (uint64_t)1 << sq;
            char piece = 0;

            for (int i = 0; i < 12; i++)
            {
                if (*(pieces[i].pos_bb) & mask)
                {
                    piece = pieces[i].symbol;
                }
            }
            board[rank][file] = piece;
        }
    }
}

int main()
{
    char board[8][8];

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Chessboard",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        600, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    Piece *pieces = get_pieces();
    bitboards_to_board(pieces, board);

    int sel_file = -1;
    int sel_row = -1;

    int needs_redraw = 1;
    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                sel_file = event.button.x / SQUARE_SIZE;
                sel_row = 7 - (event.button.y / SQUARE_SIZE);
                Piece *pieces = get_pieces();
                needs_redraw = 1;
            }
        }

        if (needs_redraw)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render_board(renderer, board, pieces, SQUARE_SIZE, sel_file, sel_row);
            needs_redraw = 0;
        }
        SDL_Delay(10);
    }

    return 0;
}