#ifndef IMAGES_H
#define IMAGES_H

#include "raylib.h"

#include "globals.h"

#include <string>
#include <cassert>

void load_fonts() {
    menu_font = LoadFontEx(nullptr, 128, nullptr, 0);
}

void unload_fonts() {
    UnloadFont(menu_font);
}

void load_images() {
    player_regular = LoadTexture("data/images/player.png");
    player_invert = LoadTexture("data/images/player_inv.png");
    floor1       = LoadTexture("data/images/floor/floor.png");
    floor2       = LoadTexture("data/images/floor/grass.png");
    floor3       = LoadTexture("data/images/floor/tiles.png");
    goal_image        = LoadTexture("data/images/goal.png");
    box_image         = LoadTexture("data/images/box.png");
    box_on_goal_image = LoadTexture("data/images/box-on-goal.png");
    house1            = LoadTexture("data/images/wall/house1.png");
    house2            = LoadTexture("data/images/wall/house2.png");
    house3            = LoadTexture("data/images/wall/house3.png");
    city1             = LoadTexture("data/images/wall/town1.png");
    city2             = LoadTexture("data/images/wall/town2.png");
    city3             = LoadTexture("data/images/wall/town3.png");
    wilderness1       = LoadTexture("data/images/wall/wilderness1.png");
    wilderness2       = LoadTexture("data/images/wall/wilderness2.png");
    wilderness3       = LoadTexture("data/images/wall/wilderness3.png");

    candle_off        = LoadTexture("data/images/candle_off.png");
    candle_on         = LoadTexture("data/images/candle_on.png");
    blood         = LoadTexture("data/images/blood.png");

    player.setImage(player_regular);
}

void unload_images() {
    UnloadTexture(goal_image);
    UnloadTexture(box_image);
    UnloadTexture(box_on_goal_image);
}

void draw_image(Texture2D image, float x, float y, float size) {
    draw_image(image, x, y, size, size);
}

void draw_image(Texture2D image, float x, float y, float width, float height) {
    Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    Rectangle destination = { x, y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

sprite load_sprite(
    const std::string &file_name_prefix,
    const std::string &file_name_suffix,
    size_t frame_count,
    bool loop,
    size_t frames_to_skip
) {
    assert(frame_count < 100);

    sprite result = {
        frame_count, frames_to_skip, 0, 0, loop, 0, new Texture2D[frame_count]
    };

    for (size_t i = 0; i < frame_count; ++i) {
        std::string file_name;
        if (frame_count < 10) {
            file_name = file_name_prefix;
            file_name += std::to_string(i);
            file_name += file_name_suffix;
        } else {
            file_name = file_name_prefix;
            file_name += i < 10 ? ("0" + std::to_string(i)) : std::to_string(i);
            file_name += file_name_suffix;
        }
        result.frames[i] = LoadTexture(file_name.c_str());
    }

    return result;
}

void unload_sprite(sprite &sprite) {
    assert(sprite.frames != nullptr);

    for (size_t i = 0; i < sprite.frame_count; ++i) {
        UnloadTexture(sprite.frames[i]);
    }
    delete[] sprite.frames;
    sprite.frames = nullptr;
}

void draw_sprite(sprite &sprite, float x, float y, float size) {
    draw_sprite(sprite, x, y, size, size);
}

void draw_sprite(sprite &sprite, float x, float y, float width, float height) {
    draw_image(sprite.frames[sprite.frame_index], x, y, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

#endif // IMAGES_H
