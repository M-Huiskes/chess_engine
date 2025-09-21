#include <SDL2/SDL.h>

const int SQUARE_SIZE = 75;

void render_board(SDL_Renderer *renderer, int square_size)
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

            // if (board[y][x] != 0)
            // {
            //     if (board[y][x] >= 'A' && board[y][x] <= 'Z')
            //     {
            //         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white pieces
            //     }
            //     else
            //     {
            //         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black pieces
            //     }
            //     SDL_Rect pieceRect = {
            //         x * square_size + square_size / 4,
            //         y * square_size + square_size / 4,
            //         square_size / 2,
            //         square_size / 2};
            //     SDL_RenderFillRect(renderer, &pieceRect);
            // }
        }
    }
    SDL_RenderPresent(renderer); // update window
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Chessboard",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        600, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x / SQUARE_SIZE;
                int y = event.button.y / SQUARE_SIZE;
                printf("Clicked on square: %c%d\n", 'a' + x, 8 - y);
            }
        }

        render_board(renderer, SQUARE_SIZE);
    }
}