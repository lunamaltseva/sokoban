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
    while (!movements.empty()) movements.pop();
    this->row = row;
    this->column = column;
}

void Player::move(size_t dx, size_t dy) {
    vector2 next(row + dy, column + dx);

    if (!level.is_cell_inside(next.x, next.y)) return;

    char &next_cell = level.get_cell(next.x, next.y);

    if (next_cell == Level::FLOOR || next_cell == Level::GOAL) {
        row    = static_cast<size_t>(next.x);
        column = static_cast<size_t>(next.y);
    }

    else if (next_cell == Level::BOX || next_cell == Level::BOX_ON_GOAL) {
        vector2 target(next.x + dy, next.y + dx);
        if (!level.is_cell_inside(target.x, target.y)) return;

        char &target_cell = level.get_cell(target.x, target.y);

        if (target_cell == Level::FLOOR || target_cell == Level::GOAL) {
            next_cell = next_cell == Level::BOX ? Level::FLOOR : Level::GOAL;
            if (target_cell == Level::FLOOR) target_cell = Level::BOX;
            else {
                target_cell = Level::BOX_ON_GOAL;
                play_sound(goal_sound);
            }

            row    = static_cast<size_t>(next.x);
            column = static_cast<size_t>(next.y);

            level.if_solved();
        }
    }

    movements.push({dx, dy});
}

#endif // PLAYER_H
