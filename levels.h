#ifndef LEVELS_H
#define LEVELS_H

#include "globals.h"
#include "player.h"
#include "graphics.h"

#include <cstddef>

void LevelManager::load(size_t offset) {
    game_state = GAME_STATE;

    index+=offset;
    if (index >= levels.size()) {
        index = 0;
        game_state = VICTORY_STATE;
        create_victory_menu_background();
    }

    Level* level = LevelManager::getInstance();

    level->rows    = levels[index].rows;
    level->columns = levels[index].columns;
    level->data = new char[level->rows * level->columns];

    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            char cell = levels[index].data[row * level->columns + column];
            if (cell == Level::PLAYER) {
                level->set_cell(row, column, Level::FLOOR);
                player.spawn(row, column);
            } else if (cell == Level::PLAYER_ON_GOAL) {
                level->set_cell(row, column, Level::GOAL);
                player.spawn(row, column);
            } else {
                level->set_cell(row, column, cell);
            }
        }
    }

    derive_graphics_metrics_from_loaded_level();
}

int Level::count(char object) {
    int instances = 0;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            char cell = data[row * columns + column];
            if (cell == object) instances++;
        }
    }
    return instances;
}

void LevelManager::unload() {
    Level* level = LevelManager::getInstance();

    level->rows    = 0;
    level->columns = 0;

    delete[] level->data;
    level->data = nullptr;
}

bool Level::is_cell_inside(int row, int column) {
    return row < rows && column < columns;
}

char& Level::get_cell(size_t row, size_t column) {
    return data[row * columns + column];
}

void Level::set_cell(size_t row, size_t column, char cell) {
    data[row * columns + column] = cell;
}

void Level::if_solved() {
    bool level_solved = true;
    for (size_t row = 0; level_solved && row < height(); ++row) {
        for (size_t column = 0; level_solved && column < width(); ++column) {
            char cell_to_test = get_cell(row, column);
            if (cell_to_test == Level::GOAL) {
                level_solved = false;
            }
        }
    }

    if (level_solved) {
        levelManager.unload();
        levelManager.load(1);
    }
}

#endif // LEVELS_H
