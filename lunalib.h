#ifndef LUNALIB_H
#define LUNALIB_H

#include "raylib.h"
#include <string>
#include <cstddef>
#include <vector>
#include <functional>

size_t game_frame = 0;
size_t runtime = 0;

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

int key_recently_pressed = 0;
bool is_key(int check) {
    return IsKeyDown(check) && key_recently_pressed == check;
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

void draw_image(Texture2D image, float x, float y, float size);

class Slide {
public:
    Slide(const std::string &text, const Texture2D picture)
        : caption{Text(text, WHITE, 30.0f, {0.9f, 0.5f})}, picture{picture} { }
    void draw() {
        caption.draw();
        float minimum = std::min(GetScreenHeight(), GetScreenWidth())*0.8f;
        draw_image(picture, (GetScreenWidth()-minimum)*0.5f, (GetScreenHeight()-minimum)*0.5f, minimum);
    }
private:
    Text caption;
    Texture2D picture;
};

class Slideshow {
public:
    explicit Slideshow(int time) : timePerSlide(time) {}
    explicit Slideshow(std::vector<Slide> &slides, int time) : slides(slides), timePerSlide(time) {}
    void add(const Slide &slide) { slides.push_back(slide); }
    void draw() {
        slides[itr].draw();
        if (runtime > timePerSlide) itr++;
    }
private:
    int timePerSlide;
    int itr = 0;
    std::vector<Slide> slides;
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

    Level() {}
    Level(size_t height, size_t width, char *data)
            : rows(height), columns(width), data{data} {}

    bool is_cell_inside(int row, int column);
    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char cell);
    void if_solved();
    int count(char object);

    size_t height() { return rows; }
    size_t width() { return columns; }
    friend class LevelManager;
private:
    size_t rows, columns;
    char *data;
};

class LevelManager {
public:
    explicit LevelManager(std::vector<Level> &list) { levels = list; }
    static void add(Level &level) {levels.push_back(level);}

    static Level* getInstance() {
        if (instance == nullptr)
            instance = new Level();
        return instance;
    }

    void load(size_t offset = 0);
    void unload();
    void reset() { index = 0; unload();};
    static size_t get_index() { return index; }
    friend void draw_loaded_level();
private:
    static Level* instance;
    static size_t index;
    static std::vector<Level> levels;
};

Level* LevelManager::instance = nullptr;
size_t LevelManager::index = 0;
std::vector<Level> LevelManager::levels;

class Player {
public:
    void setImage(Texture2D texture);
    void spawn(size_t row, size_t column);
    void move(size_t dx, size_t dy);
    void undo_move();
    void draw();

    size_t get_row() {return row;}
    size_t get_column() {return column;}
    Texture2D getImage() {return image;}
    int getSteps() {return movements.size();}

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

extern Sound scroll;
extern Sound forward;
extern Sound backout;

class Menu {
public:
    Menu(std::vector<Option> entry, std::function<void()> backward, Color colorActive = WHITE, Color colorInactive = GRAY, float size = 50.0f, Vector2 offset = {0.5f,0.5f}, Vector2 offsetAdd = {0.0f, 0.1f}, float spacing = 1.0f, Font *font = &menu_font)
            : entry(entry), backward(backward), colorActive(colorActive), colorInactive(colorInactive), size(size), font(font), spacing(spacing), offsetPercentInitial(offset), offsetPercentAdditional(offsetAdd), selection(0) {}

    void draw() {
        for (int i = 0; i < entry.size(); i++) {
            Text text(entry[i].text, (i == selection ? colorActive : colorInactive), size, {offsetPercentInitial.x + offsetPercentAdditional.x*i, offsetPercentInitial.y + offsetPercentAdditional.y*i}, spacing, font);
            text.draw();
        }
    }

    void run() {
        if (mv_down())         {if (entry.size() <= ++selection) selection = 0; PlaySound(scroll);}
        else if (mv_up())      {if (0 > --selection) selection = entry.size()-1;PlaySound(scroll);}
        else if (mv_forward()) {entry[selection].forward();PlaySound(forward);}
        else if (mv_back())    {backward();PlaySound(backout);}
        this->draw();
    }

    int selected() {
        return selection;
    }
protected:
    std::vector<Option> entry;
    std::function<void()> backward;
    int selection;
    float size, spacing;
    Vector2 offsetPercentInitial, offsetPercentAdditional;
    Color colorActive, colorInactive;
    Font* font;
};

struct Parameters {
    int value;
    enum values {key, speed};
    values valueType;
};

class OptionsMenu : public Menu {
public:
    OptionsMenu(std::vector<Option> entry, std::function<void()> backward, Color colorActive = WHITE, Color colorInactive = GRAY, float size = 50.0f, Vector2 offset = {0.5f,0.5f}, Vector2 offsetAdd = {0.0f, 0.1f}, float spacing = 1.0f, Font *font = &menu_font)
    : Menu(entry, backward, colorActive, colorInactive, size, offset, offsetAdd, spacing, font) {}

    void draw() {
        Menu::draw();
        for (int i = 0; i < entry.size(); i++) {
            Text text(parameters[i].valueType == Parameters::key ? std::to_string(parameters[i].value) : std::to_string((char)parameters[i].value), (i == selection && selected ? colorActive : colorInactive), size, {offset + offsetPercentAdditional.x*i, offsetPercentInitial.y + offsetPercentAdditional.y*i}, spacing, font);
            text.draw();
        }
    }

    int getKey() {
        int stroke = GetKeyPressed();
        if (stroke != 0) {
            parameters[selection].value = stroke;
        }
        return parameters[selection].value;
    }

    void increaseDecrease() {
        int &val = parameters[selection].value;

        if (val>=1 && val<=60) {
            if (mv_down() && val!=60) val++;
            else if (mv_up() && val!=1) val--;
        }
    }

    void run() {
        if (!selected) {
            if (mv_back()) { backward(); PlaySound(backout); }
            else if (mv_forward()) {selected = true; PlaySound(forward);}
            else if (mv_down()) { if (entry.size() <= ++selection) selection = 0; PlaySound(scroll);}
            else if (mv_up()) { if (0 > --selection) selection = entry.size() - 1; PlaySound(scroll);}
        }
        else {
            if (mv_back() || mv_forward()) {selected = false; PlaySound(backout);}
            else {
                entry[selection].forward();
            }
        }

        this->draw();
    }

    int getValue(int index) {
        return parameters[index].value;
    }
protected:
    std::vector<Parameters> parameters = {{KEY_W, Parameters::key},{KEY_A, Parameters::key},{KEY_S, Parameters::key},{KEY_D, Parameters::key}, {3, Parameters::speed}};
    bool selected;
    float offset = 0.5f;
};

#endif //LUNALIB_H
