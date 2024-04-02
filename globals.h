#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#include <string>
#include <cstddef>
#include <stack>

struct vector2 {
    vector2() : x(0), y(0) {};
    vector2(size_t row, size_t column) : x(row), y(column) {}
    size_t x, y;
};

/* Levels */

class Level {
public:
    static const char WALL           = '#',
                      FLOOR          = '-',
                      BOX            = '$',
                      BOX_ON_GOAL    = '*',
                      GOAL           = '.',
                      PLAYER         = '@',
                      PLAYER_ON_GOAL = '+';

    Level() {level_index = 0;}
    Level(size_t height, size_t width, char *data)
        : rows(height), columns(width), data{data} {}

    void load(size_t offset = 0);
    void unload();
    bool is_cell_inside(int row, int column);
    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char cell);
    void if_solved();

    size_t get_index() { return level_index; }
    size_t height() { return rows; }
    size_t width() {return columns; }
private:
    size_t rows, columns;
    char *data;
    size_t level_index;
};

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

class Player {
public:
    void setImage(Texture2D texture);
    void spawn(size_t row, size_t column);
    void move(size_t dx, size_t dy);
    void draw();

    size_t get_row() {return row;}
    size_t get_column() {return column;}

    ~Player() {
        UnloadTexture(image);
    }
private:
    size_t row, column;
    Texture2D image;
    std::stack<vector2> movements;
};

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

/* Fonts */

Font menu_font;

/* Menu Text Parameters */

const std::string MENU_TITLE     = "Sokoban";
const float MENU_TITLE_FONT_SIZE = 200.0f;
const float MENU_TITLE_Y_SHIFT   = 10.0f;
const Color MENU_TITLE_COLOR     = RED;

const std::string MENU_SUBTITLE     = "Press Enter to start the game";
const float MENU_SUBTITLE_FONT_SIZE = 30.0f;
const float MENU_SUBTITLE_Y_SHIFT   = 80.0f;
const Color MENU_SUBTITLE_COLOR     = WHITE;

/* Game Text Parameters */

const float GAME_LEVEL_FONT_SIZE = 70.0f;
const float GAME_LEVEL_Y_SHIFT   = 30.0f;
const Color GAME_LEVEL_COLOR1    = GRAY;
const Color GAME_LEVEL_COLOR2    = WHITE;

/* Images and Sprites */

Texture2D wall_image;
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

/* Reload Request Text Parameters */

const std::string RELOAD_REQ_TITLE     = "Press R to restart the level";
const float RELOAD_REQ_TITLE_FONT_SIZE = 30.0f;
const float RELOAD_REQ_TITLE_Y_SHIFT   = 0.0f;
const Color RELOAD_REQ_TITLE_COLOR     = WHITE;

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

/* Victory Menu Text Parameters */

const std::string VICTORY_TITLE     = "You Won!";
const float VICTORY_TITLE_FONT_SIZE = 200.0f;
const float VICTORY_TITLE_Y_SHIFT   = 10.0f;
const Color VICTORY_TITLE_COLOR     = RED;

const std::string VICTORY_SUBTITLE     = "Press Enter to go back to menu";
const float VICTORY_SUBTITLE_FONT_SIZE = 30.0f;
const float VICTORY_SUBTITLE_Y_SHIFT   = 80.0f;
const Color VICTORY_SUBTITLE_COLOR     = WHITE;

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
