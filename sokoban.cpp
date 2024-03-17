#include "raylib.h"

// Forward Declarations
#include "globals.h"

// Gameplay
#include "levels.h"
#include "figure.h"

// Visuals
#include "drawing.h"
#include "animation.h"
#include "ui_ux.h"
#include "assets.h"

void updateGame() {
    controlMusic();
    controlEsc();
    controlMenus();


    switch(gameState) {
        case INTRO_STATE:                                                     // avoid accidental skips
            if((isAffirmativeButtonPressed() || isNegativeButtonPressed()) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) toMainMenu();
            break;

        case MENU_STATE:
            interpretSelection(mainMenu);
            break;

        case CHOOSE_LEVEL_STATE:
            interpretSelection(chooseLevelMenu);
            break;

        case TUTORIAL_STATE:
            if (isAffirmativeButtonPressed() || isNegativeButtonPressed()) {
                gameState = MENU_STATE;
                PlaySound(backOutSound);
            }
            break;

        case GAME_STATE:
            if      (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)   ) { movePlayer( 0, -1); }
            else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) ) { movePlayer(-1,  0); }
            else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) ) { movePlayer( 0,  1); }
            else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { movePlayer( 1,  0); }
            if (isNegativeButtonPressed()) {
                PlaySound(backOutSound);
                gameState = PAUSED_STATE;
                exitFromAnimation = false;
            }
            break;

        case PAUSED_STATE:
            interpretSelection(pauseMenu);
            break;

        case ANIMATION_STATE:
            if (isNegativeButtonPressed()) {
                PlaySound(backOutSound);
                gameState = PAUSED_STATE;
                exitFromAnimation = true;
            }
            break;

        case GAME_OVER_STATE:
            interpretSelection(gameOverMenu);
            break;

        case VICTORY_STATE:
            interpretSelection(victoryMenu);
            break;
    }
}

void drawGame() {
    ClearBackground(BLACK);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    Vector2 playerPos = {
            camera.zoom*((static_cast<float>(player.column) * cellSize + shiftToCenterCellByX) - (screenWidth-cellSize)*0.5f),
            camera.zoom*((static_cast<float>(player.row) * cellSize + shiftToCenterCellByY) - (screenHeight-cellSize)*0.5f)};
    camera.target = playerPos;

    BeginMode2D(camera);
    switch(gameState) {
        case INTRO_STATE:
            drawIntro();
            break;

        case MENU_STATE:
            gameFrame++;
            drawMainMenu();
            drawMenu(mainMenu);
            fade(static_cast<int>(gameFrame), 0, 60, -static_cast<float>(animationDuration) / 30.0f, 256);
            break;

        case CHOOSE_LEVEL_STATE:
            gameFrame++;
            drawMainMenu();
            drawChooseLevelMenu();
            break;

        case TUTORIAL_STATE:
            gameFrame++;
            drawTutorial();
            break;

        case GAME_STATE:
            gameFrame++;
            /* Draw the board */
            drawLevel();
            /* Draw the figures */
            drawFigure(player);
            /* Draw the GUI */
            drawGUI();
            break;

        case ANIMATION_STATE:
            gameFrame++;
            drawAnimation();
            break;

        case PAUSED_STATE:
            drawPauseMenu();
            break;

        case VICTORY_STATE:
            drawVictoryMenu();
            break;

        case GAME_OVER_STATE:
            drawGameOverMenu();
            break;
    }
    EndMode2D();
}

int main() {                       // Since we are dealing with pixel arts, antialiasing is actually detrimental
    SetConfigFlags(FLAG_VSYNC_HINT /*| FLAG_MSAA_4X_HINT*/);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(1), "Dungeon");
    SetTargetFPS(60);
    ToggleFullscreen();
    HideCursor();

    InitAudioDevice();
    initGraphics();

    loadFonts();
    loadImages();
    loadSounds();

    while (!WindowShouldClose()) {
        BeginDrawing();

        updateGame();
        drawGame();

        EndDrawing();
    }

    unloadFonts();
    unloadImages();
    unloadSounds();

    delete[] loadLevel;
    CloseAudioDevice();
    CloseWindow();

    return 0;
}