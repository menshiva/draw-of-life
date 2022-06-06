#ifndef DRAWOFLIFE_CELLS_H
#define DRAWOFLIFE_CELLS_H

#include <stdint.h>
#include <assert.h>

// cell data packing - 32 bits unsigned integer
// 00000000    00000000     00000000    000          0000           0
// r channel   g channel    b channel  unused  alive neighbors  is alive

typedef uint32_t Cell;
typedef Cell Cells[CELLS_NUM];

static inline uint32_t getCellHexColor(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return cells[idx] >> 8u;
}

static inline void setCellHexColor(Cells cells, int idx, uint32_t color) {
    assert(idx >= 0 && idx < CELLS_NUM);
    cells[idx] = (color << 8u) | (cells[idx] & 0xFFu);
}

static inline uint8_t getCellAliveNeighborsNum(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return (cells[idx] >> 1u) & 0xFu;
}

static inline void setCellNeighborState(Cells cells, int idx, bool alive) {
    assert(idx >= 0 && idx < CELLS_NUM);
    if (alive)
        cells[idx] += 0x2u;
    else
        cells[idx] -= 0x2u;
    assert(getCellAliveNeighborsNum(cells, idx) < 9);
}

static inline bool isCellAlive(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return cells[idx] & 0x1u;
}

static void generateCellNeighborIndices(int idx, int neighborIndices[8]) {
    int y = idx / CELLS_X_NUM;
    int x = idx % CELLS_X_NUM;

    int xLeft = (x > 0) ? (x - 1) : (CELLS_X_NUM - 1);
    int xRight = (x < CELLS_X_NUM - 1) ? (x + 1) : 0;
    int yAbove = (y > 0) ? (y - 1) : (CELLS_Y_NUM - 1);
    int yBelow = (y < CELLS_Y_NUM - 1) ? (y + 1) : 0;

    neighborIndices[0] = yAbove * CELLS_X_NUM + xLeft;
    neighborIndices[1] = yAbove * CELLS_X_NUM + x;
    neighborIndices[2] = yAbove * CELLS_X_NUM + xRight;
    neighborIndices[3] = y * CELLS_X_NUM + xLeft;
    neighborIndices[4] = y * CELLS_X_NUM + xRight;
    neighborIndices[5] = yBelow * CELLS_X_NUM + xLeft;
    neighborIndices[6] = yBelow * CELLS_X_NUM + x;
    neighborIndices[7] = yBelow * CELLS_X_NUM + xRight;
}

static void setCellState(Cells cells, int idx, int *neighborIndices, bool alive) {
    assert(idx >= 0 && idx < CELLS_NUM);
    if (alive)
        cells[idx] |= 0x1u;
    else
        cells[idx] &= ~0x1u;
    for (int i = 0; i < 8; ++i)
        setCellNeighborState(cells, neighborIndices[i], alive);
}

static uint32_t generateCellHexColor(const Cells cells, int idx, int *neighborIndices) {
    assert(idx >= 0 && idx < CELLS_NUM);
    float avgR = 0.0f, avgG = 0.0f, avgB = 0.0f;
    float aliveNum = 0.0f;

    for (int i = 0; i < 8; ++i) {
        if (isCellAlive(cells, neighborIndices[i])) {
            uint32_t neighborColor = getCellHexColor(cells, neighborIndices[i]);
            float b = (float) (neighborColor & 0xFFu);
            neighborColor >>= 8;
            float g = (float) (neighborColor & 0xFFu);
            neighborColor >>= 8;
            float r = (float) (neighborColor & 0xFFu);
            avgR += r * r, avgG += g * g, avgB += b * b;
            aliveNum += 1.0f;
        }
    }

    assert(aliveNum > 0);
    avgR = sqrtf(avgR / aliveNum), avgG = sqrtf(avgG / aliveNum), avgB = sqrtf(avgB / aliveNum);

    uint32_t avgColor = (uint32_t) avgR;
    avgColor = (avgColor << 8u) | (uint8_t) avgG;
    avgColor = (avgColor << 8u) | (uint8_t) avgB;

    return avgColor;
}

static void createCellsNewGeneration(const Cells currentGen, Cells newGen) {
    for (int i = 0; i < CELLS_NUM; ++i) {
        uint8_t aliveNeighborsNum = getCellAliveNeighborsNum(currentGen, i);
        if (!isCellAlive(currentGen, i)) {
            if (aliveNeighborsNum == 3) {
                int neighborIndices[8];
                generateCellNeighborIndices(i, neighborIndices);
                setCellState(newGen, i, neighborIndices, true);
                setCellHexColor(newGen, i, generateCellHexColor(currentGen, i, neighborIndices));
            }
        }
        else {
            if (aliveNeighborsNum < 2 || aliveNeighborsNum > 3) {
                int neighborIndices[8];
                generateCellNeighborIndices(i, neighborIndices);
                setCellState(newGen, i, neighborIndices, false);
            }
        }
    }
}

static inline void clearCells(Cells cells) {
    memset(cells, 0, sizeof(Cells));
}

#endif
