#ifndef FIGURE_H
#define FIGURE_H

#include <cstdlib>
#include <cmath>
#include "globals.h"

/* General */

float simplifyNumber(float num) {
    if (num > 0.0f)      return  1.0f;
    else if (num < 0.0f) return -1.0f;
    else                 return  0.0f;
}

/* Player */

void movePlayer(int dx, int dy) {
    if (player.cell == QUICKTILE) {
        level.data[player.row*level.columns+player.column] = WALL;
        PlaySound(rand()%2 == 0 ? quicktileBreakSound0 : quicktileBreakSound1);
        player.shouldJump = true;
    }

    int nextPlayerColumn = static_cast<int>(player.column) + dx;
    int nextPlayerRow = static_cast<int>(player.row)       + dy;

    player.cell = level.data[nextPlayerRow * level.columns + nextPlayerColumn];

    initMovement(player, player.row, player.column, nextPlayerRow, nextPlayerColumn);
    player.row = static_cast<size_t>(nextPlayerRow);
    player.column = static_cast<size_t>(nextPlayerColumn);
    level.moves--;

    if (level.moves % 5 == 0) {
        if      (level.moves > 15)  PlaySound(rand()%2 == 0 ? clockTickSound0 : clockTickSound1);
        else if (level.moves == 15) PlaySound(clockHighSound);
        else if (level.moves == 10) PlaySound(clockNormalSound);
    }

    if (level.moves <= 5 )
        PlaySound(clockLowSound);
}

void playerCompleteMovement() {
    player.shouldJump = false;

    /* Trigger ending? */
    if (levelIndex == 4 && isExitOpen) {
        player.position = static_cast<int>(player.row * level.columns + player.column);
    }

    gameState = GAME_STATE;

    player.cell = level.data[player.row * level.columns + player.column];

    switch (player.cell) {
        case WALL:
            PlaySound(figureFallSound);
            playAnimation = playerFalling;
            initAnimation();
            break;
        case KEY:
            PlaySound(pickupKeySound);
            player.keys++;
            level.data[player.row * level.columns + player.column] = FLOOR;
            if (player.keys == level.keys) {
                isExitOpen = true;
                PlaySound(exitOpenSound);
            }
            break;
        case MARBLE:
            PlaySound(marbleTopupSound);
            level.moves += (LEVELS[levelIndex].moves/2);
            level.moves = (level.moves > LEVELS[levelIndex].moves ?  LEVELS[levelIndex].moves : level.moves);
            level.data[player.row * level.columns + player.column] = FLOOR;
            break;
        case EXIT:
            if (isExitOpen) {
                playAnimation = levelEnding;
                initAnimation();
                PlaySound(puzzleCompleteSound);
                return;
            }
            break;
    }

    if (level.moves == 0) {
        playAnimation = playerOutOfMoves;
        PlaySound(levelBreakSound);
        initAnimation();
        return;
    }
}
#endif //FIGURE_H
