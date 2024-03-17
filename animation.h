#ifndef ANIMATION_H
#define ANIMATION_H

#include <cmath>
#include "raylib.h"
#include "globals.h"

void initMovement(Figure figure, size_t rowFrom, size_t columnFrom, size_t rowTo, size_t columnTo) {
    positionFrom = {static_cast<float>(columnFrom) * cellSize + shiftToCenterCellByX,
                    static_cast<float>(rowFrom) * cellSize + shiftToCenterCellByY};
    positionTo   = {static_cast<float>(columnTo) * cellSize + shiftToCenterCellByX,
                    static_cast<float>(rowTo) * cellSize + shiftToCenterCellByY};
    path = {positionTo.x-positionFrom.x, positionTo.y-positionFrom.y};
    playAnimation = playerMoving;
    initAnimation();
}

void initAnimation() {
    framesPerEntity = 60; //using GetFPS() here breaks the movement
    beginningFrame = gameFrame;
    gameState = ANIMATION_STATE;
}

void animateMovement(Figure &figure, int i) {
    /* Draw Elements On Screen */
    drawGUI();
    drawLevel();

    drawImage(figure.image,
              positionFrom.x + 0.5f*(path.x * (1.0f-cosf( static_cast<float>(i)/static_cast<float>(framesPerEntity)*PI ))),
              positionFrom.y + 0.5f*(path.y * (1.0f-cosf( static_cast<float>(i)/static_cast<float>(framesPerEntity)*PI ))) - cellSize * 0.3125f - (figure.shouldJump ? ((1.0f-cosf(static_cast<float>(i)/static_cast<float>(framesPerEntity)*2.0f*PI))*0.5f)*cellSize*0.4f : 0.0f),
              cellSize);
}

void animateFalling(Figure &figure, int i) {
    drawImage(figure.image,
              static_cast<float>(figure.column) * cellSize + shiftToCenterCellByX,
              static_cast<float>(figure.row)    * cellSize + shiftToCenterCellByY - cellSize * 0.3125f + powf(static_cast<float>(i)/60.0f, 2.0f)*cellSize*9.8f,
              cellSize);

    /* Draw Elements On Screen */
    drawLevel();
    drawGUI();
}

void endAnimation(int i) {
    // Fade out, then black out
    if (i < 90) {
        fade(i, static_cast<float>(animationDuration)/45.0f);
    } else DrawRectangle(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), {0, 0, 0, 255});

    // Restart the Level
    if (i > animationDuration) {
        restartLevel();
    }
}

void drawAnimation() {
    int i = static_cast<int>(static_cast<float>(gameFrame - beginningFrame));

    switch (playAnimation) {
        case fromMenu:
        {
            const float titleSize = MENU_TITLE_SIZE * screenScale;
            const float figureSize = titleSize * 0.2f;
            const float amplitude = 15.0f * screenScale;
            float figureOffsetY = amplitude*2.0f*sinf(static_cast<float>(gameFrame%70)*(PI/35.0f));

            Vector2 titlePosition = {(screenWidth-titleSize)*0.5f, 0};
            Vector2 figurePosition = {titlePosition.x+(titleSize-figureSize)*0.5f, titlePosition.y+(titleSize-figureSize)*0.2f};

            drawImage(playerImage, figurePosition.x, figurePosition.y + figureOffsetY - amplitude*2.0f + pow(i/60.0f, 2)*cellSize*9.8f, figureSize);
        }

            if (i < 50) fade(i, static_cast<float>(animationDuration)/25.0f);
            else        DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 255} );

            if (i > 60) loadNextLevel(0);

            break;

        case playerMoving:
            if (level.moves <= 5) drawLevelHalo(i);
            animateMovement(player, i);
            if (i >= framesPerEntity - 1) playerCompleteMovement();
            break;

        case playerOutOfMoves:
            shiftToCenterCellByY += powf(static_cast<float>(i)/60.0f, 2.0f)*cellSize*0.3f;

            drawLevel();
            drawFigures();
            drawGUI();

            endAnimation(i);
            break;

        case playerFalling:
            animateFalling(player, i);
            endAnimation(i);
            break;

        case playerKilled:
            drawLevel();
            drawGUI();
            drawFigures();

            endAnimation(i);
            break;

        case levelStarting:
            // No flashing lights
            i = i == 0 ? 1 : i;

            drawLevel();
            drawGUI();

            fade(i, 0, 120, -(static_cast<float>(animationDuration)/60.0f), 256);

            drawImage(player.image,
                      static_cast<float>(player.column) * cellSize + shiftToCenterCellByX,
                      (static_cast<float>(player.row) * cellSize + shiftToCenterCellByY - cellSize * 0.3125f) - static_cast<float>(animationDuration-i)*(screenHeight*0.025f),
                      cellSize);

            if (i >= animationDuration) completeLevelDrawing();
            break;

        case levelEnding:
            i = i == 0 ? 1 : i;

            drawLevel();
            drawGUI();

            if (i <= 32)
                drawImage(player.image,
                          static_cast<float>(player.column) * cellSize + shiftToCenterCellByX,
                          (static_cast<float>(player.row) * cellSize + shiftToCenterCellByY - cellSize * 0.3125f) - static_cast<float>((sinf(static_cast<float>(i)/60.0f*2.0f*PI)*0.5f)*cellSize),
                          cellSize);

            fade(i, 0, 120, (static_cast<float>(animationDuration)/60.0f), 0);

            if (i >= animationDuration) loadNextLevel(1);
            break;

        case gameOver:
            drawAnimatedText(gameOverTitle);
            if (gameOverTitle.textIndex >= gameOverTitle.entries.size()) {
                gameOverTitle.textIndex = 0;
                gameState = GAME_OVER_STATE;
            }
            break;
    }
}

#endif //ANIMATION_H
