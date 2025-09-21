#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SQUARE_SIZE = 75;
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
    SDL_Texture *texture;
} PieceBB;

typedef struct
{
    int file;
    int row;
} Square;

// void print_bitboard(PieceBB pieces[])
// {
//     for (int rank = 7; rank >= 0; rank--)
//     {
//         printf("%d ", rank + 1);
//         for (int file = 0; file < 8; file++)
//         {
//             int sq = rank * 8 + file;
//             uint64_t mask = (uint64_t)1 << sq;
//             char piece = '.';

//             for (int i = 0; i < 12; i++)
//             {
//                 if (*(pieces[i].bb) & mask)
//                 {
//                     piece = pieces[i].symbol;
//                 }
//             }
//             printf("%c", piece);
//         }
//         printf("\n");
//     }
//     printf("  abcdefgh\n");
// }

// Square get_input_square(void)
// {
//     char input[100]; // big enough to hold the whole line

//     while (1)
//     {
//         printf("Enter square (i.e. h1): ");

//         if (!fgets(input, sizeof(input), stdin))
//         {
//             // Input error or EOF
//             Square invalid = {-1, -1};
//             return invalid;
//         }

//         // Remove trailing newline if it exists
//         input[strcspn(input, "\n")] = '\0';

//         // Check length (must be exactly 2: e.g. 'h' and '1')
//         if (strlen(input) != 2 || input[0] > 'h' || input[1] > '8')
//         {
//             printf("Invalid input, please enter like h1.\n");
//             continue; // ask again
//         }

//         char file_char = input[0];
//         char row_char = input[1];

//         int file = file_char - FILE_OFFSET;
//         int row = row_char - ROW_OFFSET;

//         Square square = {file, row};
//         return square;
//     }
// }

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
PieceBB *find_piece_by_position(PieceBB pieces[], int position)
{
    uint64_t mask = ((int64_t)1 << position);
    for (int i = 0; i < 12; i++)
    {
        if (*(pieces[i].bb) & mask)
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

void make_move(Square input, Square output, PieceBB pieces[])
{
    int input_pos = get_position(input.file, input.row);
    int output_pos = get_position(output.file, output.row);

    // Hier nog checken of er geen null pointer terugkomt
    PieceBB *piece = find_piece_by_position(pieces, input_pos);
    unset_bit(piece->bb, input_pos);
    set_bit(piece->bb, output_pos);
}

void render_board(SDL_Renderer *renderer, char board[8][8], PieceBB pieces[], int square_size)
{
    // Draw squares
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Rect rect = {x * square_size, y * square_size, square_size, square_size};
            if ((x + y) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255); // light
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255); // dark
            }
            SDL_RenderFillRect(renderer, &rect);

            if (board[y][x] != 0)
            {
                SDL_Texture *tex = NULL;
                for (int i = 0; i < 12; i++)
                {
                    if (pieces[i].symbol == board[y][x])
                    {
                        tex = pieces[i].texture;
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
    SDL_RenderPresent(renderer); // update window
}

void bitboards_to_board(PieceBB pieces[], char board[8][8])
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
                if (*(pieces[i].bb) & mask)
                {
                    piece = pieces[i].symbol;
                }
            }
            board[rank][file] = piece;
        }
    }
}

void add_image_textures(PieceBB pieces[], SDL_Renderer *renderer)
{
    for (int i = 0; i < 12; i++)
    {
        char path[64];
        switch (pieces[i].symbol)
        {
        case 'P':
            strcpy(path, "images/pawn_w.png");
            break;
        case 'p':
            strcpy(path, "images/pawn_b.png");
            break;
        case 'R':
            strcpy(path, "images/rook_w.png");
            break;
        case 'r':
            strcpy(path, "images/rook_b.png");
            break;
        case 'N':
            strcpy(path, "images/knight_w.png");
            break;
        case 'n':
            strcpy(path, "images/knight_b.png");
            break;
        case 'B':
            strcpy(path, "images/bishop_w.png");
            break;
        case 'b':
            strcpy(path, "images/bishop_b.png");
            break;
        case 'Q':
            strcpy(path, "images/queen_w.png");
            break;
        case 'q':
            strcpy(path, "images/queen_b.png");
            break;
        case 'K':
            strcpy(path, "images/king_w.png");
            break;
        case 'k':
            strcpy(path, "images/king_b.png");
            break;
        }
        SDL_Surface *surface = IMG_Load(path);
        if (!surface)
        {
            printf("Failed to load image %s: %s\n", path, IMG_GetError());
            continue;
        }
        pieces[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

int main()
{
    PieceBB pieces[12] = {
        {&WHITE_PAWNS, 'P', NULL},
        {&BLACK_PAWNS, 'p', NULL},
        {&WHITE_ROOKS, 'R', NULL},
        {&BLACK_ROOKS, 'r', NULL},
        {&WHITE_KNIGHTS, 'N', NULL},
        {&BLACK_KNIGHTS, 'n', NULL},
        {&WHITE_BISHOPS, 'B', NULL},
        {&BLACK_BISHOPS, 'b', NULL},
        {&WHITE_QUEEN, 'Q', NULL},
        {&BLACK_QUEEN, 'q', NULL},
        {&WHITE_KING, 'K', NULL},
        {&BLACK_KING, 'k', NULL},
    };

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
    add_image_textures(pieces, renderer);

    int running = 1;
    SDL_Event event;
    bitboards_to_board(pieces, board);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            // if (event.type == SDL_MOUSEBUTTONDOWN)
            // {
            //     int x = event.button.x / SQUARE_SIZE;
            //     int y = event.button.y / SQUARE_SIZE;
            // }
        }
        render_board(renderer, board, pieces, SQUARE_SIZE);
    }

    return 0;
}