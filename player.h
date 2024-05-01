#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "levels.h"
#include "sounds.h"

#include <cstddef>

void Player::setImage(Texture2D texture) {
    image = texture;
}

void Player::spawn(size_t row, size_t column) {
    movements.clear();
    was_box_moved.clear();
    this->row = row;
    this->column = column;
}

void Player::move(size_t dx, size_t dy) {
    Level *level = LevelManager::getInstance();

    vector2 next(row + dy, column + dx);
    vector2 target(next.x + dy, next.y + dx);

    if (!level->is_cell_inside(next.x, next.y)) return;

    char &next_cell = level->get_cell(next.x, next.y);
    char &target_cell = level->get_cell(target.x, target.y);

    if (!(next_cell == Level::FLOOR || next_cell == Level::BOX || next_cell == Level::GOAL || next_cell == Level::BOX_ON_GOAL)) return;
    if (next_cell == Level::BOX || next_cell == Level::BOX_ON_GOAL)
        if (!(target_cell == Level::FLOOR || target_cell == Level::GOAL))
            return;

    if (next_cell == Level::BOX || next_cell == Level::BOX_ON_GOAL &&
        target_cell == Level::FLOOR || target_cell == Level::GOAL) was_box_moved.push_back(true);
    else was_box_moved.push_back(false);

    if (next_cell == Level::FLOOR || next_cell == Level::GOAL) {
        row    = next.x;
        column = next.y;
    }

    else if (next_cell == Level::BOX || next_cell == Level::BOX_ON_GOAL) {
        if (!level->is_cell_inside(target.x, target.y)) return;

        if (target_cell == Level::FLOOR || target_cell == Level::GOAL) {
            next_cell = next_cell == Level::BOX ? Level::FLOOR : Level::GOAL;
            if (target_cell == Level::FLOOR) target_cell = Level::BOX;
            else {
                target_cell = Level::BOX_ON_GOAL;
                play_sound(goal_sound);
            }

            row    = static_cast<size_t>(next.x);
            column = static_cast<size_t>(next.y);

            level->if_solved();
        }
    }

    movements.push_back({dx, dy});
    runtime = 0;
}

void Player::undo_move() {
    if (movements.empty()) return;

    Level *level = LevelManager::getInstance();

    vector2 d_pos(movements[movements.size()-1]);
    movements.pop_back();

    bool move_box = was_box_moved[was_box_moved.size()-1];
    was_box_moved.pop_back();

    vector2 ahead(row + d_pos.y, column + d_pos.x);
    char &ahead_cell = level->get_cell(ahead.x, ahead.y);

    if (move_box && ahead_cell == Level::BOX) {
        level->set_cell(row, column, Level::BOX);
        ahead_cell = Level::FLOOR;
    }

    else if (move_box && ahead_cell == Level::BOX_ON_GOAL) {
        ahead_cell = Level::GOAL;

        if (level->get_cell(row, column) == Level::GOAL) {
            level->set_cell(row, column, Level::BOX_ON_GOAL);
        }
        else
            level->set_cell(row, column, Level::BOX);
    }

    row    -= d_pos.y;
    column -= d_pos.x;
    runtime = 0;
}

#endif // PLAYER_H
