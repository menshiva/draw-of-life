#ifndef DRAWOFLIFE_CELLS_PANEL_H
#define DRAWOFLIFE_CELLS_PANEL_H

bool shouldDrawGrid = true;

static inline void toggleGrid(void) {
    shouldDrawGrid = !shouldDrawGrid;
}

static void drawGrid(SDL_Window *window, SDL_Renderer *renderer) {
    if (!shouldDrawGrid) return;
    scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(GRID_COLOR)));
    for (int x = CELL_SIZE; x < CELLS_PANEL_WIDTH; x += CELL_SIZE)
        scc(window, renderer, SDL_RenderDrawLine(renderer, x, 0, x, CELLS_PANEL_HEIGHT));
    for (int y = CELL_SIZE; y <= CELLS_PANEL_HEIGHT; y += CELL_SIZE)
        scc(window, renderer, SDL_RenderDrawLine(renderer, 0, y, CELLS_PANEL_WIDTH, y));
}

static void drawCells(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    for (int y = 0; y < CELLS_Y_NUM; ++y) {
        for (int x = 0; x < CELLS_X_NUM; ++x) {
            if (isCellAlive(y, x)) {
                rect.y = y * CELL_SIZE;
                rect.x = x * CELL_SIZE;
                scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(getCellHexColor(y, x))));
                scc(window, renderer, SDL_RenderFillRect(renderer, &rect));
            }
        }
    }
}

static void onCellsPanelDraw(int screenY, int screenX) {
    int y = screenY / CELL_SIZE;
    int x = screenX / CELL_SIZE;
    if (!isCellAlive(y, x))
        setCellState(y, x, true);
    setCellHexColor(y, x, getSelectedColor());
}

static void onCellsPanelErase(int screenY, int screenX) {
    int y = screenY / CELL_SIZE;
    int x = screenX / CELL_SIZE;
    if (isCellAlive(y, x))
        setCellState(y, x, false);
}

#endif
