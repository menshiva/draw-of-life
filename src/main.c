#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "prefs.h"

#define CELLS_NUM                       (CELLS_Y_NUM * CELLS_X_NUM)
#define CELLS_PANEL_HEIGHT              (CELLS_Y_NUM * CELL_SIZE)
#define CELLS_PANEL_WIDTH               (CELLS_X_NUM * CELL_SIZE)

#define COLOR_PANEL_HEIGHT              COLOR_PALETTE_CELL_SIZE
#define COLOR_PANEL_WIDTH               (COLOR_PALETTE_NUM * COLOR_PALETTE_CELL_SIZE)

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
#include "gens_buffer.h"
#include "color_panel.h"
#include "cells_panel.h"

void redrawRenderer(SDL_Window *window, SDL_Renderer *renderer) {
    scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR)));
    scc(window, renderer, SDL_RenderClear(renderer));
    drawPalette(window, renderer);
    drawGrid(window, renderer);
    drawCells(getCurrentGeneration(), window, renderer);
    SDL_RenderPresent(renderer);
}

void printControls(void) {
    puts("Mouse controls:");
    puts("\tLeft mouse button click (hold) on grid:\n\t\tDraw with selected color (mark cell(s) as alive).");
    puts("\tLeft mouse button click on color palette:\n\t\tChange draw color.");
    puts("\tRight mouse button click (hold) on grid:\n\t\tErase (mark cell(s) as dead).");
    puts("Keyboard controls:");
    puts("\tLeft arrow / Right arrow:\n\t\tChange draw color.");
    puts("\tUp arrow / Down arrow:\n\t\tNext / previous generation.");
    puts("\tG:\n\t\tToggle grid.");
    puts("\tC:\n\t\tClear all (mark all cells as dead).");
    puts("\tSpace:\n\t\tStart / stop animation with generating next epochs.");
    puts("\tEsc:\n\t\tClose app.");
}

int main() {
    // init
    scc(NULL, NULL, SDL_Init(SDL_INIT_VIDEO));
    int windowWidth = (CELLS_PANEL_WIDTH > COLOR_PANEL_WIDTH) ? CELLS_PANEL_WIDTH : COLOR_PANEL_WIDTH;
    SDL_Window *window = (SDL_Window*) scp(NULL, NULL, SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, CELLS_PANEL_HEIGHT + COLOR_PANEL_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
    ));
    SDL_Renderer *renderer = (SDL_Renderer*) scp(window, NULL, SDL_CreateRenderer(
            window, -1,
            SDL_RENDERER_SOFTWARE
    ));

    bool isRendererDirty = true;
    bool isLeftMouseBtnPressed = false;
    bool isRightMouseBtnPressed = false;
    bool isAnimationEnabled = false;
    uint32_t lastMs = SDL_GetTicks();
    SDL_Event event;

    printControls();

    // event loop
    while (true) {
        if (isAnimationEnabled) {
            size_t currentMs = SDL_GetTicks();
            if (currentMs - lastMs >= ANIM_DURATION_MS) {
                lastMs = currentMs;
                if (!setNextGeneration())
                    createCellsNewGeneration(getPreviousGeneration(), getCurrentGeneration());
                isRendererDirty = true;
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) {
                // quit sdl
                sdlQuit(window, renderer);
                return 0;
            }
            switch (event.type) {
                case SDL_KEYUP: {
                    if (event.key.keysym.sym == SDLK_g) {
                        toggleGrid();
                        isRendererDirty = true;
                    }
                    else if (!isAnimationEnabled && event.key.keysym.sym == SDLK_c) {
                        clearCells(getCurrentGeneration());
                        setCurrentGenerationModified();
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
                    else if (event.key.keysym.sym == SDLK_SPACE) {
                        isAnimationEnabled = !isAnimationEnabled;
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    if (!isAnimationEnabled) {
                        if (event.key.keysym.sym == SDLK_UP) {
                            if (!setNextGeneration())
                                createCellsNewGeneration(getPreviousGeneration(), getCurrentGeneration());
                            isRendererDirty = true;
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) {
                            if (setPreviousGeneration())
                                isRendererDirty = true;
                        }
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
                            onCellsPanelDraw(getCurrentGeneration(), y, x);
                            setCurrentGenerationModified();
                            isRendererDirty = true;
                        }
                        else if (event.button.button == SDL_BUTTON_RIGHT) {
                            isRightMouseBtnPressed = true;
                            onCellsPanelErase(getCurrentGeneration(), y, x);
                            setCurrentGenerationModified();
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
                            onCellsPanelDraw(getCurrentGeneration(), y, x);
                            setCurrentGenerationModified();
                            isRendererDirty = true;
                        }
                        else if (isRightMouseBtnPressed) {
                            onCellsPanelErase(getCurrentGeneration(), y, x);
                            setCurrentGenerationModified();
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
}
