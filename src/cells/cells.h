#ifndef DRAWOFLIFE_CELLS_H
#define DRAWOFLIFE_CELLS_H

#include <stdint.h>
#include <assert.h>

// cell data packing - 32 bits unsigned integer
//  000      00000000    00000000     00000000         0000           0
// unused    r channel   g channel    b channel  alive neighbours  is alive

typedef uint32_t Cell;
typedef Cell Cells[CELLS_Y_NUM][CELLS_X_NUM];
static Cells cells;

static inline uint32_t getCellHexColor(int y, int x) {
    return cells[y][x] >> 5;
}

static inline void setCellHexColor(int y, int x, uint32_t color) {
    cells[y][x] |= (color << 5);
}

static inline uint8_t getCellNeighboursNum(int y, int x) {
    return (cells[y][x] >> 1) & 0xF;
}

static inline void setCellNeighbour(int y, int x, bool add) {
    if (add)
        cells[y][x] += 0x2;
    else
        cells[y][x] -= 0x2;
    assert(getCellNeighboursNum(y, x) < 9);
}

static inline bool isCellAlive(int y, int x) {
    return cells[y][x] & 0x1;
}

static void setCellState(int y, int x, bool alive) {
    if (alive)
        cells[y][x] |= 0x1;
    else
        cells[y][x] &= ~0x1;

    int yAbove = (y == 0) ? CELLS_Y_NUM - 1 : y - 1;
    int yBelow = (y + 1) % CELLS_Y_NUM;
    int xLeft = (x == 0) ? CELLS_X_NUM - 1 : x - 1;
    int xRight = (x + 1) % CELLS_X_NUM;

    setCellNeighbour(yAbove, xLeft, alive);
    setCellNeighbour(yAbove, x, alive);
    setCellNeighbour(yAbove, xRight, alive);
    setCellNeighbour(y, xLeft, alive);
    setCellNeighbour(y, xRight, alive);
    setCellNeighbour(yBelow, xLeft, alive);
    setCellNeighbour(yBelow, x, alive);
    setCellNeighbour(yBelow, xRight, alive);
}

#endif
