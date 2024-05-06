#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

#include "globals.h"
#include "images.h"
#include "levels.h"
#include "player.h"
#include "utilities.h"
#include "lunalib.h"

#include <string>
#include <cmath>
#include <cstdlib>

void Text::draw() {
    dimensions = MeasureTextEx(*font, text.c_str(), size, spacing);

    Vector2 pos = {
            (screen_width *offsetPercent.x)-(0.5f*dimensions.x) + 0.75f*((cosf(rand()%10*game_frame)+sinf(rand()%4*game_frame))),
            (screen_height*offsetPercent.y)-(0.5f*dimensions.y)
    };
    DrawTextEx(*font, text.c_str(), pos, dimensions.y, spacing, color);
}

void draw_GUI() {
    Level *level = LevelManager::getInstance();
    int boxes = level->count(Level::BOX);
    int boxes_on_goals = level->count(Level::BOX_ON_GOAL);
    boxes+=boxes_on_goals;

    for (int i = 0; i < boxes; i++) {
        Vector2 position = {screen_width*0.5f+((float)(rand()%(int)(0.5f*screen_width))-0.25f*screen_width), screen_height-cell_size};
        draw_image((i < boxes_on_goals ? candle_on : candle_off), position.x, position.y, cell_size);
    }

    for (int i = 0; i < (totalMoves/100); i++) {
        Vector2 position = {(float)(rand()%(int)(screen_width-cell_size)), screen_height-cell_size};
        draw_image(blood, position.x, position.y, cell_size);
    }
}

void draw_Menu() {
    Text title("Catastrophic", RED, 80.0f, {0.2f, 0.2f}, 4.0f);
    Text byline("By @lunamaltseva", GRAY, 30.0f, {0.2f, 0.85f}, 2.0f);
    title.draw();
    byline.draw();
    int minimum = std::min(screen_width, screen_height);
    float scale = minimum*0.25f;
    draw_image(goal_image, screen_width-(scale*1.75f), screen_height-(scale*1.5f), scale);
    draw_image(player.getImage(), screen_width-(scale*3.0f), screen_height-(scale*1.5f), scale);
}

void derive_graphics_metrics_from_loaded_level() {
    Level *level = LevelManager::getInstance();

    screen_width  = static_cast<float>(GetScreenWidth());
    screen_height = static_cast<float>(GetScreenHeight());

    cell_size = std::min(
        screen_width  / static_cast<float>(level->width()),
        screen_height / static_cast<float>(level->height())
    ) * CELL_SCALE;
    screen_scale = std::min(
        screen_width,
        screen_height
    ) / SCREEN_SCALE_DIVISOR;
    float level_width  = static_cast<float>(level->width()) * cell_size;
    float level_height = static_cast<float>(level->height())    * cell_size;
    shift_to_center_cell_by_x = (screen_width - level_width)   * 0.5f;
    shift_to_center_cell_by_y = (screen_height - level_height) * 0.3f;
}

Texture2D wall_image() {
    Texture2D result;

    int access = rand()%3+1;
    switch(LevelManager::get_index()) {
        case 0:
            switch(access) {
                case 1: result = house1; break;
                case 2: result = house2; break;
                case 3: result = house3; break;
            }
            break;
        case 1:
            switch(access) {
                case 1: result = city1; break;
                case 2: result = city2; break;
                case 3: result = city3; break;
            }
            break;
        case 2:
            switch(access) {
                case 1: result = wilderness1; break;
                case 2: result = wilderness2; break;
                case 3: result = wilderness3; break;
            }
            break;
    }

    return result;
}

Texture2D floorImage() {
    Texture2D result;
    int access = rand()%4+1;
    switch(access) {
    case 1: case 2: result = floor1; break;
        case 3: result = floor2; break;
        case 4: result = floor3; break;
    }
    return result;
}

void draw_loaded_level() {
    ClearBackground(BLACK);
    Level *level = LevelManager::getInstance();
    srand(1);

    for (size_t row = 0; row < level->height(); ++row) {
        for (size_t column = 0; column < level->width(); ++column) {
            float x = shift_to_center_cell_by_x + static_cast<float>(column) * cell_size;
            float y = shift_to_center_cell_by_y + static_cast<float>(row)    * cell_size;

            char cell = level->get_cell(row, column);
            switch (cell) {
                case Level::WALL:
                    draw_image(wall_image(), x, y, cell_size);
                    break;
                case Level::GOAL:
                    if (!(player.get_row() == row && player.get_column() == column))
                        draw_image(goal_image, x, y, cell_size);
                    break;
                case Level::BOX:
                    draw_image(floorImage(), x, y, cell_size);
                    draw_image(box_image, x, y, cell_size);
                    break;
                case Level::BOX_ON_GOAL:
                    draw_image(box_on_goal_image, x, y, cell_size);
                    break;
                case Level::FLOOR:
                    draw_image(floorImage(), x, y, cell_size);
                    break;
                default:
                    break;
            }
        }
    }
}

void Player::draw() {
    float x = shift_to_center_cell_by_x + static_cast<float>(column) * cell_size;
    float y = shift_to_center_cell_by_y + static_cast<float>(row)    * cell_size;
    draw_image(image, x, y, cell_size);
}

void create_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x  = rand_up_to(screen_width);
        ball.y  = rand_up_to(screen_height);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dx *= screen_scale;
        if (abs(ball.dx) < 0E-1) ball.dx = 1.0f;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dy *= screen_scale;
        if (abs(ball.dy) < 0E-1) ball.dy = 1.0f;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
        ball.radius *= screen_scale;
    }

    /* Clear both the front buffer and the back buffer to avoid ghosting of the game graphics. */
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
}

void animate_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x += ball.dx;
        if (ball.x - ball.radius < 0 ||
            ball.x + ball.radius >= screen_width) {
            ball.dx = -ball.dx;
        }
        ball.y += ball.dy;
        if (ball.y - ball.radius < 0 ||
            ball.y + ball.radius >= screen_height) {
            ball.dy = -ball.dy;
        }
    }
}

void draw_victory_menu_background() {
    for (auto &ball : victory_balls) {
        DrawCircleV({ ball.x, ball.y }, ball.radius, VICTORY_BALL_COLOR);
    }
}

void draw_victory_menu() {
    DrawRectangle(
        0, 0,
        static_cast<int>(screen_width), static_cast<int>(screen_height),
        { 0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY }
    );

    animate_victory_menu_background();
    draw_victory_menu_background();

    victory_title.draw();
    victory_subtitle.draw();
}

#endif // GRAPHICS_H
