#ifndef DRAWOFLIFE_COLOR_PANEL_H
#define DRAWOFLIFE_COLOR_PANEL_H

static uint8_t selectedColorIdx;

static inline uint32_t getSelectedColor(void) {
    return COLOR_PALETTE[selectedColorIdx];
}

static void drawPalette(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect rect = {0, CELLS_PANEL_HEIGHT, COLOR_PALETTE_CELL_SIZE, COLOR_PALETTE_CELL_SIZE};
    scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(GRID_COLOR)));
    rect.x = selectedColorIdx * COLOR_PALETTE_CELL_SIZE;
    scc(window, renderer, SDL_RenderFillRect(renderer, &rect));

    rect.x = COLOR_SELECT_BORDER_THICKNESS;
    rect.y += COLOR_SELECT_BORDER_THICKNESS;
    rect.w = rect.h = COLOR_PALETTE_CELL_SIZE - COLOR_SELECT_BORDER_THICKNESS * 2;
    for (int i = 0; i < COLOR_PALETTE_NUM; ++i) {
        scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(COLOR_PALETTE[i])));
        scc(window, renderer, SDL_RenderFillRect(renderer, &rect));
        rect.x += COLOR_PALETTE_CELL_SIZE;
    }
}

#endif
