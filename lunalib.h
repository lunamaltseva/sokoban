#ifndef LUNALIB_H
#define LUNALIB_H

#include "raylib.h"
#include <string>
#include <cstddef>
#include <vector>
#include <functional>

bool mv_down() {
    return (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT));
}

bool mv_up() {
    return (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT));
}

bool mv_forward() {
    return (IsKeyPressed(KEY_ENTER));
}

bool mv_back() {
    return (IsKeyPressed(KEY_ESCAPE));
}

Font menu_font;
class Text {
public:
    Text(const std::string text, Color color = WHITE, float size = 50.0f, Vector2 offset = {0.5f,0.5f}, float spacing = 1.0f, Font *font = &menu_font)
            : text(text), color(color), size(size), font(font), spacing(spacing), offsetPercent(offset) {}
    void draw();
private:
    std::string text;
    float size, spacing;
    Vector2 offsetPercent, dimensions;
    Color color;
    Font* font;
};

struct vector2 {
    vector2() : x(0), y(0) {};
    vector2(size_t row, size_t column) : x(row), y(column) {}
    size_t x, y;
};

class Level {
public:
    static const char WALL = '#',
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
    void reset() {level_index = 0; unload();};
    int count(char object);

    size_t get_index() { return level_index; }
    size_t height() { return rows; }
    size_t width() {return columns; }
private:
    size_t rows, columns;
    char *data;
    size_t level_index;
};

class Player {
public:
    void setImage(Texture2D texture);
    void spawn(size_t row, size_t column);
    void move(size_t dx, size_t dy);
    void undo_move();
    void draw();

    size_t get_row() {return row;}
    size_t get_column() {return column;}

    ~Player() {
        UnloadTexture(image);
    }
private:
    size_t row, column;
    Texture2D image;
    std::vector<vector2> movements;
    std::vector<bool> was_box_moved;
};

struct Option {
    std::string text;
    std::function<void()> forward;
};

class Menu {
public:
    Menu(std::vector<Option> entry, std::function<void()> backward, Color colorActive = WHITE, Color colorInactive = GRAY, float size = 50.0f, Vector2 offset = {0.5f,0.5f}, Vector2 offsetAdd = {0.0f, 0.1f}, float spacing = 1.0f, Font *font = &menu_font)
            : entry(entry), backward(backward), colorActive(colorActive), colorInactive(colorInactive), size(size), font(font), spacing(spacing), offsetPercentInitial(offset), offsetPercentAdditional(offsetAdd), selection(0) {}

    void draw() {
        for (int i = 0; i < entry.size(); i++) {
            Text text(
                    entry[i].text,
                    (i == selection ? colorActive : colorInactive),
                    size,
                    {offsetPercentInitial.x + offsetPercentAdditional.x*i, offsetPercentInitial.y + offsetPercentAdditional.y*i},
                    spacing,
                    font
            );
            text.draw();
        }
    }

    void run() {
        if (mv_down())    {if (entry.size() <= ++selection) selection = 0; }
        else if (mv_up()) {if (0 > --selection) selection = entry.size()-1;}
        else if (mv_forward()) {entry[selection].forward();}
        else if (mv_back()) {backward();}
        this->draw();
    }

    int selected() {
        return selection;
    }
private:
    std::vector<Option> entry;
    std::function<void()> backward;
    int selection;
    float size, spacing;
    Vector2 offsetPercentInitial, offsetPercentAdditional;
    Color colorActive, colorInactive;
    Font* font;
};

#endif //LUNALIB_H
