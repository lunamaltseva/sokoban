#ifndef LEVELS_H
#define LEVELS_H

#include "globals.h"

// Make the menu not break
void initGraphics() {
    createLevelInstance();
    deriveGraphicsMetricsFromLoadedLevel();
}

// Pre-generation
void initLevel() {
    // Return to default values
    player.image = playerImage;
    player.isToBeKilled = false;
    player.isKilled = false;
    player.keys = 0;

    // Avoid quicktile bug
    player.cell = ' ';

    isExitOpen = false;
}

void placePlayer() {
    for (size_t row = 0; row < level.rows; row++) {
        for (size_t column = 0; column < level.columns; column++) {
            char cell = level.data[row*level.columns + column];
            if (cell == ENTRANCE) {
                player.row = row;
                player.column = column;
            }
        }
    }
}

bool scanForEnemies() {
    for (size_t row = 0; row < level.rows; row++) {
        for (size_t column = 0; column < level.columns; column++) {
            char cell = level.data[row*level.columns + column];
            if (cell == PAWN || cell == ROOK || cell == QUEEN) {
                return true;
            }
        }
    }
    return false;
}


void loadLevelConfig() {
    switch(levelIndex + 1) {
        case 2:
        case 4:
            break;
    }
}


// Generation
void createLevelInstance() {
    delete[] loadLevel;
    loadLevel = new char[LEVELS[levelIndex].rows*LEVELS[levelIndex].columns];
    for(int i = 0; i < LEVELS[levelIndex].rows*LEVELS[levelIndex].columns; i++)
        loadLevel[i] = LEVELS[levelIndex].data[i];

    level = { LEVELS[levelIndex].rows, LEVELS[levelIndex].columns, LEVELS[levelIndex].moves, LEVELS[levelIndex].keys, loadLevel };
}

void loadNextLevel(int jump) {
    levelIndex+=jump;
    initLevel();

    // Is game over?

    createLevelInstance();

    placePlayer();

    loadLevelConfig();

    deriveGraphicsMetricsFromLoadedLevel();

    playAnimation = levelStarting;
    initAnimation();
}

void completeLevelDrawing() {
    PlaySound(playerLandedSound);
    gameState = GAME_STATE;
}

void listLevelElements(bool array[], size_t size, size_t checkLevelIndex) {
    for (int i = 0; i < size; i++)
        array[i] = false;

    for (size_t row = 0; row < LEVELS[checkLevelIndex].rows; row++) {
        for (size_t column = 0; column < LEVELS[checkLevelIndex].columns; column++) {
            char cell = LEVELS[checkLevelIndex].data[row*LEVELS[checkLevelIndex].columns + column];
            if      (cell == KEY)       array[keyElement]       = true;
            else if (cell == MARBLE)    array[marbleElement]    = true;
            else if (cell == QUICKTILE) array[quicktileElement] = true;
            else if (cell == PAWN)      array[pawnElement]      = true;
            else if (cell == ROOK)      array[rookElement]      = true;
            else if (cell == QUEEN)     array[queenElement]     = true;
        }
    }
}

void restartLevel() {
    player.lives--;
    loadNextLevel(0);
}

#endif // LEVELS_H
