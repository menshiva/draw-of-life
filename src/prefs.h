#ifndef DRAWOFLIFE_PREFS_H
#define DRAWOFLIFE_PREFS_H

#define WINDOW_TITLE        "Draw of Life"
#define BACKGROUND_COLOR    0x000000

// should be at least 2
#define GENERATIONS_BUFFER_SIZE         128
#define ANIM_DURATION_MS                150

#define CELLS_Y_NUM                     60
#define CELLS_X_NUM                     100
#define CELL_SIZE                       16
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

#endif // DRAWOFLIFE_PREFS_H
