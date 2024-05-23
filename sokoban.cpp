#include "raylib.h"
#include "globals.h"
#include "levels.h"
#include "player.h"
#include "draw.h"
#include "assets.h"
#include "lunalib.h"
#include "run.h"

void update_game() {
    switch (gameState) {
        case INTRO_STATE:
            play(theme);
            if (!intro.draw() || IsKeyPressed(KEY_ENTER)) {
                gameState = MENU_STATE;
                Animation::transition(Animation::fade_in);
            }
            break;
        case MENU_STATE: case OPTION_STATE: case SELECT_LEVEL_STATE:
            play(mainTheme);
            break;

        case GAME_STATE:
            playLevelMusic();

            if (displayPrompt) {
                if (mv_forward() || mv_back()) {
                    displayPrompt = nullptr;
                }
            }
            else if (Animation::state() == Animation::none) {
                int key_pressed;
                key_pressed = GetKeyPressed();

                if (key_pressed != 0) key_recently_pressed = key_pressed;

                bool isAnyDown;
                isAnyDown = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D) ||
                            IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) ||
                            IsKeyDown(KEY_U);

                int speed;
                speed = 60 / optionsMenu.getValue(5);

                if (!isAnyDown) runtime = speed;
                else if (++runtime < speed)
                    break;

                if (is_key(optionsMenu.getValue(0))) {
                    player.move(0, -1);
                    return;
                } else if (is_key(optionsMenu.getValue(2))) {
                    player.move(0, 1);
                    return;
                } else if (is_key(optionsMenu.getValue(1))) {
                    player.move(-1, 0);
                    return;
                } else if (is_key(optionsMenu.getValue(3))) {
                    player.move(1, 0);
                    return;
                } else if (is_key(optionsMenu.getValue(4))) {
                    player.undo_move();
                }
                if (mv_back()) {
                    PlaySound(backout);
                    gameState = PAUSED_STATE;
                    game_frame = 0;
                }
                if (IsKeyPressed(KEY_X) && IsKeyDown(KEY_LEFT_CONTROL)) levelManager.forceComplete();
            }
            break;
        case PAUSED_STATE:
            if (IsKeyPressed(KEY_R)) {
                levelManager.unload();
                levelManager.load(0);
                gameState = GAME_STATE;
            }
            playLevelMusic();
            break;
        case ENDING_STATE:
            if (endingGood.position() == 1 || endingBad.position() == 1 && animationFrame == 0) {
                play(ending);
            }
            if ((LevelManager::stats[0].steps + LevelManager::stats[1].steps + LevelManager::stats[2].steps < 1200 ? !endingGood.draw() : !endingBad.draw())) {
                gameState = MENU_STATE;
                Animation::transition(Animation::fade_in);
            }
            break;
    }
}

void draw_game() {
    game_frame++;
    if (Animation::state() != Animation::none) {
        Animation::run();
    }
    else switch (gameState) {
        case MENU_STATE:
            mainMenu.run();
            draw_Menu();
            break;
        case GAME_STATE:
            SeekMusicStream(theme, 0.0f);
            SeekMusicStream(mainTheme, 0.0f);
            LevelManager::draw();
            draw_GUI();
            player.draw();
            break;
        case SELECT_LEVEL_STATE:
            selectLevelMenu.run();
            mainMenu.draw();
            draw_Menu();
            break;
        case OPTION_STATE:
            mainMenu.draw();
            draw_Menu();
            optionsMenu.run();
            optionsMenuTitle.draw();
            break;
        case PAUSED_STATE:
            if (game_frame != 1) pauseMenu.run();
            pauseMenuTitle.draw();
            drawImage(bg, 0, 0, screenWidth, screenHeight);
            break;
        case STATISTIC_STATE:
            level_stats();
            levelCompletedMenu.run();
            drawImage(bg, 0, 0, screenWidth, screenHeight);
            break;
        case ENDING_STATE:
            isGameCompleted = true;
            break;
    }

    if (displayPrompt)
        displayPrompt->draw();
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Sokoban");
    SetTargetFPS(60);
    HideCursor();
    ToggleFullscreen();
    SetExitKey(0);

    loadFonts();
    loadImages();
    load_sounds();

    deriveGraphicsMetricsFromLoadedLevel();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        update_game();
        draw_game();

        EndDrawing();
    }
    CloseWindow();

    levelManager.unload();
    unload_sounds();
    unloadImages();
    unloadFonts();

    return 0;
}
