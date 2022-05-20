#ifndef DRAWOFLIFE_GENS_BUFFER_H
#define DRAWOFLIFE_GENS_BUFFER_H

static Cells gensBuffer[GENERATIONS_BUFFER_SIZE];
static int currentGenerationIdx = 0;
static int maxGenerationIdx = 0;

static inline bool setPreviousGeneration(void) {
    if (currentGenerationIdx == 0) return false;
    --currentGenerationIdx;
    return true;
}

static inline bool setNextGeneration(void) {
    if (currentGenerationIdx < maxGenerationIdx) {
        ++currentGenerationIdx;
        return true;
    }
    else if (maxGenerationIdx < GENERATIONS_BUFFER_SIZE - 1) {
        memcpy(&gensBuffer[currentGenerationIdx + 1], &gensBuffer[currentGenerationIdx], sizeof(Cells));
        ++currentGenerationIdx;
        ++maxGenerationIdx;
        return false;
    }
    else {
        memmove(&gensBuffer[0], &gensBuffer[1], (GENERATIONS_BUFFER_SIZE - 1) * sizeof(Cells));
        return false;
    }
}

static inline const Cell *getPreviousGeneration(void) {
    assert(currentGenerationIdx > 0);
    return gensBuffer[currentGenerationIdx - 1];
}

static inline Cell *getCurrentGeneration(void) {
    return gensBuffer[currentGenerationIdx];
}

static inline void setCurrentGenerationModified(void) {
    maxGenerationIdx = currentGenerationIdx;
}

#endif
