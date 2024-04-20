#include "raylib.h"

#include "globals.h"
#include "levels.h"
#include "player.h"
#include "graphics.h"
#include "images.h"
#include "sounds.h"

void update_game() {
    switch (game_state) {
        case MENU_STATE:
            break;
        case GAME_STATE:
            runtime++;
            if (runtime < 20 && GetKeyPressed()==0) break;

            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                player.move(0, -1);
                return;
            } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                player.move(0, 1);
                return;
            } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                player.move(-1, 0);
                return;
            } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                player.move(1, 0);
                return;
            } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_Z) || IsKeyDown(KEY_U)) {
                player.undo_move();
            } if (mv_back()) {
                game_state = RELOAD_REQ_STATE;
                game_frame = 0;
            }
            break;
        case RELOAD_REQ_STATE:
            if (IsKeyPressed(KEY_R)) {
                level.unload();
                level.load(0);
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

    level.unload();
    unload_sounds();
    unload_images();
    unload_fonts();

    return 0;
}
