#ifndef DRAWOFLIFE_CELLS_H
#define DRAWOFLIFE_CELLS_H

#include <stdint.h>
#include <assert.h>

// cell data packing - 32 bits unsigned integer
// 00000000    00000000     00000000    000          0000           0
// r channel   g channel    b channel  unused  alive neighbours  is alive

typedef uint32_t Cell;
typedef Cell Cells[CELLS_NUM];

static inline uint32_t getCellHexColor(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return cells[idx] >> 8;
}

static inline void setCellHexColor(Cells cells, int idx, uint32_t color) {
    assert(idx >= 0 && idx < CELLS_NUM);
    cells[idx] = (color << 8) | (cells[idx] & 0xFF);
}

static inline uint8_t getCellAliveNeighboursNum(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return (cells[idx] >> 1) & 0xF;
}

static inline void setCellNeighbourState(Cells cells, int idx, bool alive) {
    assert(idx >= 0 && idx < CELLS_NUM);
    if (alive)
        cells[idx] += 0x2;
    else
        cells[idx] -= 0x2;
    assert(getCellAliveNeighboursNum(cells, idx) < 9);
}

static inline bool isCellAlive(const Cells cells, int idx) {
    assert(idx >= 0 && idx < CELLS_NUM);
    return cells[idx] & 0x1;
}

static int *generateCellNeighbourIndices(int idx) {
    int y = idx / CELLS_X_NUM;
    int x = idx % CELLS_X_NUM;

    int xLeft = (x > 0) ? (x - 1) : (CELLS_X_NUM - 1);
    int xRight = (x < CELLS_X_NUM - 1) ? (x + 1) : 0;
    int yAbove = (y > 0) ? (y - 1) : (CELLS_Y_NUM - 1);
    int yBelow = (y < CELLS_Y_NUM - 1) ? (y + 1) : 0;

    static int neighbIndices[8];
    neighbIndices[0] = yAbove * CELLS_X_NUM + xLeft;
    neighbIndices[1] = yAbove * CELLS_X_NUM + x;
    neighbIndices[2] = yAbove * CELLS_X_NUM + xRight;
    neighbIndices[3] = y * CELLS_X_NUM + xLeft;
    neighbIndices[4] = y * CELLS_X_NUM + xRight;
    neighbIndices[5] = yBelow * CELLS_X_NUM + xLeft;
    neighbIndices[6] = yBelow * CELLS_X_NUM + x;
    neighbIndices[7] = yBelow * CELLS_X_NUM + xRight;
    return neighbIndices;
}

static void setCellState(Cells cells, int idx, int *neighbourIndices, bool alive) {
    assert(idx >= 0 && idx < CELLS_NUM);
    if (alive)
        cells[idx] |= 0x1;
    else
        cells[idx] &= ~0x1;
    for (int i = 0; i < 8; ++i)
        setCellNeighbourState(cells, neighbourIndices[i], alive);
}

static uint32_t generateCellHexColor(Cells cells, int idx, int *neighbourIndices) {
    assert(idx >= 0 && idx < CELLS_NUM);
    float avgR = 0.0f, avgG = 0.0f, avgB = 0.0f;
    float aliveNum = 0.0f;

    for (int i = 0; i < 8; ++i) {
        if (isCellAlive(cells, neighbourIndices[i])) {
            uint32_t neighbourColor = getCellHexColor(cells, neighbourIndices[i]);
            float b = (float) (neighbourColor & 0xFF);
            neighbourColor >>= 8;
            float g = (float) (neighbourColor & 0xFF);
            neighbourColor >>= 8;
            float r = (float) (neighbourColor & 0xFF);
            avgR += r * r, avgG += g * g, avgB += b * b;
            aliveNum += 1.0f;
        }
    }

    assert(aliveNum > 0);
    avgR = sqrtf(avgR / aliveNum), avgG = sqrtf(avgG / aliveNum), avgB = sqrtf(avgB / aliveNum);

    uint32_t avgColor = (uint32_t) avgR;
    avgColor = (avgColor << 8) | (uint8_t) avgG;
    avgColor = (avgColor << 8) | (uint8_t) avgB;

    return avgColor;
}

static void createCellsNextGeneration(Cells cells) {
    Cells oldGen;
    memcpy(oldGen, cells, sizeof(Cells));
    for (int i = 0; i < CELLS_NUM; ++i) {
        uint8_t aliveNeighboursNum = getCellAliveNeighboursNum(oldGen, i);
        if (!isCellAlive(oldGen, i)) {
            if (aliveNeighboursNum == 3) {
                int *neighbourIndices = generateCellNeighbourIndices(i);
                setCellState(cells, i, neighbourIndices, true);
                setCellHexColor(cells, i, generateCellHexColor(oldGen, i, neighbourIndices));
            }
        }
        else {
            if (aliveNeighboursNum < 2 || aliveNeighboursNum > 3) {
                int *neighbourIndices = generateCellNeighbourIndices(i);
                setCellState(cells, i, neighbourIndices, false);
            }
        }
    }
}

static inline void clearCells(Cells cells) {
    memset(cells, 0, sizeof(Cells));
}

#endif
