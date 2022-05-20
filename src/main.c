#include <stdbool.h>
#include "SDL2/SDL.h"

// --------! PREFERENCES !--------
#define WINDOW_TITLE        "Draw of Life"
#define BACKGROUND_COLOR    0x000000

#define CELLS_Y_NUM                     60
#define CELLS_X_NUM                     100
#define CELL_SIZE                       16
#define CELLS_NUM                       (CELLS_Y_NUM * CELLS_X_NUM)
#define CELLS_PANEL_HEIGHT              (CELLS_Y_NUM * CELL_SIZE)
#define CELLS_PANEL_WIDTH               (CELLS_X_NUM * CELL_SIZE)
#define GRID_COLOR                      0x616A6B

#define COLOR_PALETTE_CELL_SIZE         32
#define COLOR_SELECT_BORDER_THICKNESS   4
#define COLOR_PALETTE_NUM               8
static const uint32_t COLOR_PALETTE[] = {
        0xE74C3C, // red
        0x8E44AD, // purple
        0x3498DB, // blue
        0x27AE60, // green
        0xF1C40F, // yellow
        0xE67E22, // orange
        0xECF0F1, // white
        0xBDC3C7, // grey
};
#define COLOR_PANEL_HEIGHT              COLOR_PALETTE_CELL_SIZE
#define COLOR_PANEL_WIDTH               (COLOR_PALETTE_NUM * COLOR_PALETTE_CELL_SIZE)
// -------------------------------

// parsing hex color
#define HEX_COLOR(hex) \
        ((hex) >> (2 * 8)) & 0xFF, \
        ((hex) >> (1 * 8)) & 0xFF, \
        ((hex) >> (0 * 8)) & 0xFF, \
        255

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

#include "cells.h"
#include "color_panel.h"
#include "cells_panel.h"

static Cells cells;

void redrawRenderer(SDL_Window *window, SDL_Renderer *renderer) {
    scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR)));
    scc(window, renderer, SDL_RenderClear(renderer));
    drawPalette(window, renderer);
    drawGrid(window, renderer);
    drawCells(cells, window, renderer);
    SDL_RenderPresent(renderer);
}

int main(void) {
    // init
    scc(NULL, NULL, SDL_Init(SDL_INIT_VIDEO));
    int windowWidth = (CELLS_PANEL_WIDTH > COLOR_PANEL_WIDTH) ? CELLS_PANEL_WIDTH : COLOR_PANEL_WIDTH;
    SDL_Window *window = scp(NULL, NULL, SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, CELLS_PANEL_HEIGHT + COLOR_PANEL_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
    ));
    SDL_Renderer *renderer = scp(window, NULL, SDL_CreateRenderer(
            window, -1,
            SDL_RENDERER_SOFTWARE
    ));

    redrawRenderer(window, renderer);

    // event loop
    bool isRendererDirty = false;
    bool isLeftMouseBtnPressed = false;
    bool isRightMouseBtnPressed = false;
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) {
                // quit sdl
                goto exit;
            }
            switch (event.type) {
                case SDL_KEYUP: {
                    if (event.key.keysym.sym == SDLK_g) {
                        toggleGrid();
                        isRendererDirty = true;
                    }
                    else if (event.key.keysym.sym == SDLK_c) {
                        clearCells(cells);
                        isRendererDirty = true;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT) {
                        selectNextColor();
                        isRendererDirty = true;
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT) {
                        selectPreviousColor();
                        isRendererDirty = true;
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_UP) {
                        createCellsNextGeneration(cells);
                        isRendererDirty = true;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    int y = event.button.y;
                    int x = event.button.x;
                    if (y < CELLS_PANEL_HEIGHT && x < CELLS_PANEL_WIDTH) {
                        // cells panel click
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            isLeftMouseBtnPressed = true;
                            onCellsPanelDraw(cells, y, x);
                            isRendererDirty = true;
                        }
                        else if (event.button.button == SDL_BUTTON_RIGHT) {
                            isRightMouseBtnPressed = true;
                            onCellsPanelErase(cells, y, x);
                            isRendererDirty = true;
                        }
                    }
                    else if (y > CELLS_PANEL_HEIGHT && x < COLOR_PANEL_WIDTH && event.button.button == SDL_BUTTON_LEFT) {
                        // color panel click
                        onColorPanelClick(x);
                        isRendererDirty = true;
                    }
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int y = event.button.y;
                    int x = event.button.x;
                    if (y < CELLS_PANEL_HEIGHT && x < CELLS_PANEL_WIDTH) {
                        if (isLeftMouseBtnPressed) {
                            onCellsPanelDraw(cells, y, x);
                            isRendererDirty = true;
                        }
                        else if (isRightMouseBtnPressed) {
                            onCellsPanelErase(cells, y, x);
                            isRendererDirty = true;
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    if (event.button.button == SDL_BUTTON_LEFT)
                        isLeftMouseBtnPressed = false;
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        isRightMouseBtnPressed = false;
                    break;
                }
            }
        }

        if (isRendererDirty) {
            isRendererDirty = false;
            redrawRenderer(window, renderer);
        }
    }

exit:
    sdlQuit(window, renderer);
    return 0;
}
