#ifndef DRAWOFLIFE_CELLS_PANEL_H
#define DRAWOFLIFE_CELLS_PANEL_H

bool shouldDrawGrid = true;

static void drawGrid(SDL_Window *window, SDL_Renderer *renderer) {
    if (!shouldDrawGrid) return;
    scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(GRID_COLOR)));
    for (int x = CELL_SIZE; x < WINDOW_WIDTH; x += CELL_SIZE)
        scc(window, renderer, SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT));
    for (int y = CELL_SIZE; y < WINDOW_HEIGHT; y += CELL_SIZE)
        scc(window, renderer, SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y));
}

static void drawCells(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    for (int i = 0; i < CELLS_NUM; ++i) {
        if (isCellAlive(cells[i])) {
            scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(getCellHexColor(cells[i]))));
            scc(window, renderer, SDL_RenderFillRect(renderer, &rect));
        }

        rect.x += CELL_SIZE;
        if (rect.x >= WINDOW_WIDTH) {
            rect.x = 0;
            rect.y += CELL_SIZE;
        }
    }
}

#endif
