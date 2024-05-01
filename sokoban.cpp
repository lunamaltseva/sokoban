#include "raylib.h"
#include "globals.h"
#include "levels.h"
#include "player.h"
#include "graphics.h"
#include "images.h"
#include "sounds.h"
#include "lunalib.h"

void update_game() {
    switch (game_state) {
        case MENU_STATE:
            break;
        case GAME_STATE:
            int key_pressed;
            key_pressed = GetKeyPressed();

            if (key_pressed != 0)
                key_recently_pressed = key_pressed;

            bool isAnyDown; isAnyDown = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_U);

            if (!isAnyDown) runtime = 20;
            else if (++runtime < 20) break;

            if (is_key(KEY_W) || is_key(KEY_UP)) {
                player.move(0, -1);
                return;
            } else if (is_key(KEY_S) || is_key(KEY_DOWN)) {
                player.move(0, 1);
                return;
            } else if (is_key(KEY_A) || is_key(KEY_LEFT)) {
                player.move(-1, 0);
                return;
            } else if (is_key(KEY_D) || is_key(KEY_RIGHT)) {
                player.move(1, 0);
                return;
            } else if (is_key(KEY_U)) {
                player.undo_move();
            } if (mv_back()) {
                game_state = RELOAD_REQ_STATE;
                game_frame = 0;
            }
            break;
        case RELOAD_REQ_STATE:
            if (IsKeyPressed(KEY_R)) {
                levelManager.unload();
                levelManager.load(0);
                game_state = GAME_STATE;
            }
            break;
        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                game_state = MENU_STATE;
            }
            break;
    }
}

void draw_game() {
    ++game_frame;

    switch (game_state) {
        case MENU_STATE:
            main_menu.run();
            break;
        case GAME_STATE:
            draw_GUI();
            draw_loaded_level();
            player.draw();
            break;
        case SELECT_LEVEL_STATE:
            select_level_menu.run();
            main_menu.draw();
            break;
        case RELOAD_REQ_STATE:
            if (game_frame != 1) pause.run();
            break;
        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Sokoban");
    SetTargetFPS(60);
    HideCursor();
    ToggleFullscreen();
    SetExitKey(0);

    load_fonts();
    load_images();
    load_sounds();

    derive_graphics_metrics_from_loaded_level();

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
    unload_images();
    unload_fonts();

    return 0;
}
