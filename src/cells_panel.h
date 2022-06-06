#ifndef DRAWOFLIFE_CELLS_PANEL_H
#define DRAWOFLIFE_CELLS_PANEL_H

static bool shouldDrawGrid = true;

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

static void drawCells(const Cells cells, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    for (int i = 0; i < CELLS_NUM; ++i) {
        if (isCellAlive(cells, i)) {
            rect.y = (i / CELLS_X_NUM) * CELL_SIZE;
            rect.x = (i % CELLS_X_NUM) * CELL_SIZE;
            scc(window, renderer, SDL_SetRenderDrawColor(renderer, HEX_COLOR(getCellHexColor(cells, i))));
            scc(window, renderer, SDL_RenderFillRect(renderer, &rect));
        }
    }
}

static void onCellsPanelDraw(Cells cells, int screenY, int screenX) {
    int y = screenY / CELL_SIZE;
    int x = screenX / CELL_SIZE;
    int cellIdx = y * CELLS_X_NUM + x;
    if (!isCellAlive(cells, cellIdx)) {
        int neighborIndices[8];
        generateCellNeighborIndices(cellIdx, neighborIndices);
        setCellState(cells, cellIdx, neighborIndices, true);
    }
    setCellHexColor(cells, cellIdx, getSelectedColor());
}

static void onCellsPanelErase(Cells cells, int screenY, int screenX) {
    int y = screenY / CELL_SIZE;
    int x = screenX / CELL_SIZE;
    int cellIdx = y * CELLS_X_NUM + x;
    if (isCellAlive(cells, cellIdx)) {
        int neighborIndices[8];
        generateCellNeighborIndices(cellIdx, neighborIndices);
        setCellState(cells, cellIdx, neighborIndices, false);
    }
}

#endif // DRAWOFLIFE_CELLS_PANEL_H
