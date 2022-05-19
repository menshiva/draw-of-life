#include "SDL2/SDL.h"

// --------! PREFERENCES !--------
#define WINDOW_TITLE        "Draw of Life"

#define CELLS_X_NUM         100
#define CELLS_Y_NUM         60
#define CELL_SIZE           16

#define BACKGROUND_COLOR    0x000000FF
#define GRID_COLOR          0x606060FF

// do not change
#define WINDOW_WIDTH        CELLS_X_NUM * CELL_SIZE
#define WINDOW_HEIGHT       CELLS_Y_NUM * CELL_SIZE
// -------------------------------

// parsing hex color
#define HEX_COLOR(hex) \
        ((hex) >> (3 * 8)) & 0xFF, \
        ((hex) >> (2 * 8)) & 0xFF, \
        ((hex) >> (1 * 8)) & 0xFF, \
        ((hex) >> (0 * 8)) & 0xFF

void sdlQuit(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// SDL check code
void scc(SDL_Window *window, SDL_Renderer *renderer, int code) {
    if (code < 0) {
        sdlQuit(window, renderer);
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
}

// SDL check pointer
void *scp(SDL_Window *window, SDL_Renderer *renderer, void *ptr) {
    if (ptr == NULL) {
        sdlQuit(window, renderer);
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

int main(void) {
    scc(NULL, NULL, SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = scp(NULL, NULL, SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
    ));
    SDL_Renderer *renderer = scp(window, NULL, SDL_CreateRenderer(
            window, -1,
            SDL_RENDERER_SOFTWARE
    ));

    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) {
            // quit sdl
            break;
        }
    }

    sdlQuit(window, renderer);
    return 0;
}
