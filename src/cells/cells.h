#ifndef DRAWOFLIFE_CELLS_H
#define DRAWOFLIFE_CELLS_H

#include <stdint.h>

typedef uint32_t Cell;
typedef Cell Cells[CELLS_NUM];
static Cells cells;

// cell data packing - 32 bits unsigned integer
//  000      00000000    00000000     00000000         0000           0
// unused    r channel   g channel    b channel  alive neighbours  is alive

static inline uint32_t getCellHexColor(Cell cell) {
    // shift channels and append 255 alpha channel
    return (cell << 3) | 0xFF;
}

static inline uint8_t getCellNeighboursNum(Cell cell) {
    return (cell >> 1) & 0xF;
}

static inline bool isCellAlive(Cell cell) {
    return cell & 0x1;
}

#endif
