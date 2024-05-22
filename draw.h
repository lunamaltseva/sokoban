#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

#include "globals.h"
#include "assets.h"
#include "levels.h"
#include "player.h"
#include "lunalib.h"

#include <string>
#include <cmath>
#include <cstdlib>

void Text::draw() {
    dimensions = MeasureTextEx(*font, text.c_str(), size, spacing);

    Vector2 pos = {
            (screenWidth * offsetPercent.x) - (0.5f * dimensions.x) + 0.75f * ((cosf(rand() % 10 * game_frame) + sinf(rand() % 4 * game_frame))),
            (screenHeight * offsetPercent.y) - (0.5f * dimensions.y)
    };
    DrawTextEx(*font, text.c_str(), pos, dimensions.y, spacing, color);
}

void MultilineText::draw() {
    for (int i = 0; i < lines.size(); i++) {
        Text(lines[i], color, size, {offsetPercent.x+dOffset.x*i, offsetPercent.y+dOffset.y*i}, spacing, font).draw();
    }
}

void Prompt::draw() {
    Vector2 size = {0,0};
    size.y = title.size +(contents.dOffset.y * screenHeight * static_cast<float>(contents.lines.size()));
    for (auto &el : contents.lines) {
        if (MeasureTextEx(*contents.font, el.c_str(), contents.size, contents.spacing).x > size.x)
            size.x = MeasureTextEx(*contents.font, el.c_str(), contents.size, contents.spacing).x;
    }
    Vector2 sizePercent = {((screenWidth - size.x) * 0.5f) / screenWidth, ((screenHeight - size.y) * 0.5f) / screenHeight};
    title.position({0.5f, sizePercent.y});
    contents.position({0.5f, sizePercent.y+((title.size+30.0f) / screenHeight)});
    OK.position({0.5f, ((size.y + screenHeight) * 0.52f) / screenHeight});

    DrawRectangle((screenWidth - size.x) * 0.5f - 80.0f, (screenHeight - size.y) * 0.5f - 80.0f, size.x + 160.0f, size.y + 160.0f, BLACK);
    DrawRectangleLinesEx({(screenWidth - size.x) * 0.5f - 80.0f, (screenHeight - size.y) * 0.5f - 80.0f, size.x + 160.0f, size.y + 160.0f}, 5.0f, WHITE);

    title.draw();
    contents.draw();
    OK.draw();
}

void draw_GUI() {
    Level *level = LevelManager::getInstance();
    int boxes = level->count(Level::BOX);
    int boxes_on_goals = level->count(Level::BOX_ON_GOAL);
    boxes+=boxes_on_goals;

    for (int i = 0; i < boxes; i++) {
        Vector2 position = {screenWidth * 0.5f + ((float)(rand() % (int)(0.5f * screenWidth)) - 0.25f * screenWidth), screenHeight - cellSize};
        drawImage((i < boxes_on_goals ? candleOn : candleOff), position.x, position.y, cellSize);
    }

    for (int i = 0; i < ((LevelManager::stats[0].steps + LevelManager::stats[1].steps + LevelManager::stats[2].steps)/100); i++) {
        Vector2 position = {(float)(rand()%(int)(screenWidth - cellSize)), screenHeight - cellSize};
        drawImage(blood, position.x, position.y, cellSize);
    }
}

void draw_Menu() {
    mainMenuTitle.draw();
    mainMenuByline.draw();
    int minimum = std::min(screenWidth, screenHeight);
    float scale = minimum*0.25f;
    drawImage((isGameCompleted ? owner : goalImage), screenWidth - (scale * 1.75f), screenHeight - (scale * 1.5f), scale);
    drawImage(playerRegular,
              screenWidth - (scale * 3.0f) + 0.75f * ((cosf(rand() % 10 * game_frame) + sinf(rand() % 4 * game_frame))),
              screenHeight - (scale * 1.5f), scale);
}

void Menu::draw() {
    for (int i = 0; i < entry.size(); i++) {
        Text(entry[i].text, (i == selection ? colorActive : colorInactive), size, {offsetPercentInitial.x + offsetPercentAdditional.x*i, offsetPercentInitial.y + offsetPercentAdditional.y*i}, spacing, font).draw();
    }
}

void OptionsMenu::draw() {
    Menu::draw();
    for (int i = 0; i < entry.size(); i++) {
        Text(parameters[i].valueType == Parameters::speed ? std::to_string(parameters[i].value) : std::string(1, static_cast<char>(parameters[i].value)), (i == selection && selected ? colorActive : colorInactive), size, {offset + offsetPercentAdditional.x*i, offsetPercentInitial.y + offsetPercentAdditional.y*i}, spacing, font).draw();;
    }
}

void level_stats() {
    MultilineText((std::string("Level Completed!\n. .. _ __ ___ __ _ .. .\nsteps: ") + std::to_string(LevelManager::stats[LevelManager::get_index()].steps) + " \ntime: " + std::to_string(LevelManager::stats[LevelManager::get_index()].time)), {0.0f, 0.075f}, WHITE, 50.0f, {0.5f, 0.23f}).draw();
}

void deriveGraphicsMetricsFromLoadedLevel() {
    Level *level = LevelManager::getInstance();

    screenWidth  = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());

    cellSize = std::min(
            screenWidth / static_cast<float>(level->width()),
            screenHeight / static_cast<float>(level->height())
    ) * CELL_SCALE;
    screenScale = std::min(
            screenWidth,
            screenHeight
    ) / SCREEN_SCALE_DIVISOR;
    float level_width  = static_cast<float>(level->width()) * cellSize;
    float level_height = static_cast<float>(level->height()) * cellSize;
    shiftToCenterCellByX = (screenWidth - level_width) * 0.5f;
    shiftToCenterCellByY = (screenHeight - level_height) * 0.3f;
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

void LevelManager::draw() {
    ClearBackground(BLACK);
    Level *level = LevelManager::getInstance();
    srand(1);

    for (size_t row = 0; row < level->height(); ++row) {
        for (size_t column = 0; column < level->width(); ++column) {
            float x = shiftToCenterCellByX + static_cast<float>(column) * cellSize;
            float y = shiftToCenterCellByY + static_cast<float>(row) * cellSize;

            char cell = level->get_cell(row, column);
            switch (cell) {
                case Level::WALL:
                    drawImage(wall_image(), x, y, cellSize);
                    break;
                case Level::GOAL:
                    if (!(player.get_row() == row && player.get_column() == column))
                        drawImage(goalImage, x, y, cellSize);
                    break;
                case Level::BOX:
                    drawImage(floorImage(), x, y, cellSize);
                    drawImage(boxImage, x, y, cellSize);
                    break;
                case Level::BOX_ON_GOAL:
                    drawImage(boxOnGoalImage, x, y, cellSize);
                    break;
                case Level::FLOOR:
                    drawImage(floorImage(), x, y, cellSize);
                    break;
                default:
                    break;
            }
        }
    }
}

void Player::draw() {
    float x = shiftToCenterCellByX + static_cast<float>(column) * cellSize;
    float y = shiftToCenterCellByY + static_cast<float>(row) * cellSize;
    drawImage(image, x, y, cellSize);
}

void Animation::run() {
    animationFrame++;

    switch (gameState) {
        case SELECT_LEVEL_STATE:
            selectLevelMenu.draw();
        case MENU_STATE:
            mainMenu.draw();
            draw_Menu();
            break;
        case GAME_STATE:
            LevelManager::draw();
            player.draw();
            draw_GUI();
            break;
        case PAUSED_STATE:
            pauseMenu.draw();
            pauseMenuTitle.draw();
            drawImage(bg, 0, 0, screenWidth, screenHeight);
            break;
        case STATISTIC_STATE:
            level_stats();
            levelCompletedMenu.draw();
            drawImage(bg, 0, 0, screenWidth, screenHeight);
            break;
    }

    switch (state()) {
        case fade_in:
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, static_cast<unsigned char>(256.0f - (animationFrame * (256.0f / animationDuration)))});
            break;
        case fade_out:
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, static_cast<unsigned char>(animationFrame * (250.0f / animationDuration))});
            break;
    }

    if (animationFrame >= animationDuration) {
        switch (gameState) {
            case MENU_STATE:
                if (state() == fade_out) levelManager.load();
                else transition(none);
                break;
            case SELECT_LEVEL_STATE:
                levelManager.load(selectLevelMenu.selected());
                break;
            case GAME_STATE:
                if (state() == fade_in) transition(none);
                if (state() == fade_out) {
                    gameState = STATISTIC_STATE;
                    transition(fade_in);
                }
                break;
            case PAUSED_STATE:
                if (pauseMenu.selected() == 1) {
                    levelManager.unload();
                    levelManager.load();
                }
                else if (pauseMenu.selected() == 2) {
                    gameState = MENU_STATE;
                    transition(fade_in);
                    levelManager.reset();
                }
                break;
            case STATISTIC_STATE:
                if (state() == fade_in) {transition(none);break;}
                levelManager.unload();
                levelManager.load(1 - levelCompletedMenu.selected());
                break;
        }
    }
}

#endif // GRAPHICS_H
