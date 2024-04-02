#ifndef LEVELS_H
#define LEVELS_H

#include "globals.h"
#include "player.h"
#include "graphics.h"

#include <cstddef>

void Level::load(size_t offset) {
    level_index+=offset;
    if (level_index >= LEVEL_COUNT) {
        level_index = 0;
        game_state = VICTORY_STATE;
        create_victory_menu_background();
    }

    rows    = LEVELS[Level::get_index()].rows;
    columns = LEVELS[Level::get_index()].columns;
    data = new char[rows * columns];

    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            char cell = LEVELS[Level::get_index()].data[row * columns + column];
            if (cell == PLAYER) {
                this->set_cell(row, column, FLOOR);
                player.spawn(row, column);
            } else if (cell == PLAYER_ON_GOAL) {
                this->set_cell(row, column, GOAL);
                player.spawn(row, column);
            } else {
                this->set_cell(row, column, cell);
            }
        }
    }

    derive_graphics_metrics_from_loaded_level();
}

void Level::unload() {
    rows    = 0;
    columns = 0;

    delete[] data;
    data = nullptr;
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
    for (size_t row = 0; level_solved && row < level.height(); ++row) {
        for (size_t column = 0; level_solved && column < level.width(); ++column) {
            char cell_to_test = level.get_cell(row, column);
            if (cell_to_test == Level::GOAL) {
                level_solved = false;
            }
        }
    }

    if (level_solved) {
        level.unload();
        level.load(1);
        play_sound(exit_sound);
    }
}

#endif // LEVELS_H
