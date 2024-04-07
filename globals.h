#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "lunalib.h"

#include <string>
#include <cstddef>

char LEVEL_1_DATA[] = {
    ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', '#', '-', '-', '-', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', '#', '$', '-', '-', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', '#', '#', '#', '-', '-', '$', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', '#', '-', '-', '$', '-', '$', '-', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    '#', '#', '#', '-', '#', '-', '#', '#', '-', '#', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#',
    '#', '-', '-', '-', '#', '-', '#', '#', '-', '#', '#', '#', '#', '#', '-', '-', '.', '.', '#',
    '#', '-', '$', '-', '-', '$', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '.', '.', '#',
    '#', '#', '#', '#', '#', '-', '#', '#', '#', '-', '#', '@', '#', '#', '-', '-', '.', '.', '#',
    ' ', ' ', ' ', ' ', '#', '-', '-', '-', '-', '-', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

Level LEVEL_1(11, 19, LEVEL_1_DATA);

char LEVEL_2_DATA[] = {
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ',
    '#', '.', '.', '-', '-', '#', '-', '-', '-', '-', '-', '#', '#', '#',
    '#', '.', '.', '-', '-', '#', '-', '$', '-', '-', '$', '-', '-', '#',
    '#', '.', '.', '-', '-', '#', '$', '#', '#', '#', '#', '-', '-', '#',
    '#', '.', '.', '-', '-', '-', '-', '@', '-', '#', '#', '-', '-', '#',
    '#', '.', '.', '-', '-', '#', '-', '#', '-', '-', '$', '-', '#', '#',
    '#', '#', '#', '#', '#', '#', '-', '#', '#', '$', '-', '$', '-', '#',
    ' ', ' ', '#', '-', '$', '-', '-', '$', '-', '$', '-', '$', '-', '#',
    ' ', ' ', '#', '-', '-', '-', '-', '#', '-', '-', '-', '-', '-', '#',
    ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'
};

Level LEVEL_2(10, 14, LEVEL_2_DATA);

char LEVEL_3_DATA[] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '-', '-', '-', '-', '-', '@', '#', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '-', '$', '#', '$', '-', '#', '#', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '-', '$', '-', '-', '$', '#', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '$', '-', '$', '-', '#', ' ', ' ',
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '-', '$', '-', '#', '-', '#', '#', '#',
    '#', '.', '.', '.', '.', '-', '-', '#', '#', '-', '$', '-', '-', '$', '-', '-', '#',
    '#', '#', '.', '.', '.', '-', '-', '-', '-', '$', '-', '-', '$', '-', '-', '-', '#',
    '#', '.', '.', '.', '.', '-', '-', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

Level LEVEL_3 = {10, 17,LEVEL_3_DATA};

const size_t LEVEL_COUNT = 3;
Level LEVELS[LEVEL_COUNT] = {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3
};

/* Loaded Level Data */

Level level;

/* Player Data */

Player player;

/* Graphics Metrics */

const float CELL_SCALE = 0.8f; // An aesthetic parameter to add some negative space around level
const float SCREEN_SCALE_DIVISOR = 700.0f; // The divisor was found through experimentation
                                           // to scale things accordingly to look pleasant.

float screen_width;
float screen_height;
float screen_scale; // Used to scale text/UI components size and displacements based on the screen size
float cell_size;
float shift_to_center_cell_by_x;
float shift_to_center_cell_by_y;

/* Menu Text Parameters */

Menu main_menu({"Play", "Choose Level", "Settings", "Exit"}, WHITE, GRAY, 50.0f, {0.2f, 0.4f}, {0.0f, 0.075f});
Text menu_title("Catastrophic", WHITE, 75.0f, {0.2f, 0.25f});
Text menu_subtitle("Press Enter to start the game", WHITE, 30.0f, {0.5f, 0.6f});
Text pause("Press R to restart the level", WHITE, 30.0f);
Text victory_title("You Won!", RED, 200.0f);
Text victory_subtitle("Press Enter to go back to menu", WHITE, 30.0f, {0.5f, 0.6f});

/* Images and Sprites */

Texture2D wall_image1;
Texture2D wall_image2;
Texture2D floor_image;
Texture2D goal_image;
Texture2D box_image;
Texture2D box_on_goal_image;

struct sprite {
    size_t frame_count    = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index    = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

/* Sounds */

Sound goal_sound;
Sound exit_sound;

/* Victory Menu Background */

struct victory_ball {
    float x, y;
    float dx, dy;
    float radius;
};

const size_t VICTORY_BALL_COUNT     = 2000;
const float VICTORY_BALL_MAX_SPEED  = 2.0f;
const float VICTORY_BALL_MIN_RADIUS = 2.0f;
const float VICTORY_BALL_MAX_RADIUS = 3.0f;
const Color VICTORY_BALL_COLOR      = { 180, 180, 180, 255 };
const unsigned char VICTORY_BALL_TRAIL_TRANSPARENCY = 10;
victory_ball victory_balls[VICTORY_BALL_COUNT];

/* Frame Counter */

size_t game_frame = 0;

/* Game State */

enum game_state {
    MENU_STATE,
    GAME_STATE,
    RELOAD_REQ_STATE,
    VICTORY_STATE
};

game_state game_state = MENU_STATE;

/* Forward Declarations */

// GRAPHICS_H

void draw_menu();
void derive_graphics_metrics_from_loaded_level();
void draw_loaded_level();
void draw_reload_req_menu();
void create_victory_menu_background();
void animate_victory_menu_background();
void draw_victory_menu_background();
void draw_victory_menu();

// IMAGES_H

void load_fonts();
void unload_fonts();

void load_images();
void unload_images();
void draw_image(Texture2D image, float x, float y, float width, float height);
void draw_image(Texture2D image, float x, float y, float size);

sprite load_sprite(
    const std::string &file_name_prefix,
    const std::string &file_name_suffix,
    size_t frame_count = 1,
    bool loop = true,
    size_t frames_to_skip = 3
);
void unload_sprite(sprite &sprite);
void draw_sprite(sprite &sprite, float x, float y, float width, float height);
void draw_sprite(sprite &sprite, float x, float y, float size);

// SOUNDS_H

void load_sounds();
void unload_sounds();
void play_sound(Sound sound);

#endif // GLOBALS_H
