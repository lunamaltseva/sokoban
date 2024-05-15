#ifndef LUNALIB_H
#define LUNALIB_H

#include "raylib.h"
#include <string>
#include <cstddef>
#include <vector>
#include <functional>
#include <sstream>
#include <fstream>
#include <iostream>

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
    virtual void draw();
    void position(Vector2 position) {
        offsetPercent = position;
    }
    friend class Prompt;
protected:
    std::string text;
    float size, spacing;
    Vector2 offsetPercent, dimensions;
    Color color;
    Font* font;
};

class MultilineText : public Text {
public:
    MultilineText(const std::string text, Vector2 dOffset = {0.0f, 0.075f}, Color color = WHITE, float size = 50.0f, Vector2 offset = {0.5f,0.5f}, float spacing = 1.0f, Font *font = &menu_font)
        : Text("", color, size, offset, spacing, font), dOffset(dOffset) {
        std::stringstream stream(text);
        while (!stream.eof()) {
            std::string token;
            std::getline(stream, token, '\n');
            lines.push_back(token);
        }
    }
    void draw() override;
    friend class Prompt;
protected:
    Vector2 dOffset;
    std::vector<std::string> lines;
};

class Prompt {
public:
    Prompt(std::string title, std::string contents)
        : title(Text(title, WHITE, 70.0f)), contents(MultilineText(contents)) { }
    void draw();
    void run() {
        draw();
    }
private:
    Text title;
    MultilineText contents;
    Text OK = Text("continue", GRAY, 40.0f);
};

void draw_image(Texture2D image, float x, float y, float width, float height);

class Slide {
public:
    Slide(const std::string &text, const Texture2D picture)
        : caption{Text(text, WHITE, 70.0f, {0.5f, 0.9f})}, picture{picture} { }
    void draw() {
        caption.draw();
        float minimum = std::min(GetScreenHeight(), GetScreenWidth())*0.4f;
        draw_image(picture, (GetScreenWidth()-(2*minimum))*0.5f, (GetScreenHeight()-minimum)*0.5f, 2*minimum, minimum);
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
            : rows(height), columns(width), data(data) { }

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

class LevelDecoder {
public:
    static Level instantiate(std::string &line);
    static void expand(std::string &str);
    static std::vector<std::string> fillOut(std::vector<std::string> &vector, const std::string &str);
};

struct levelStatistics {
    size_t steps;
    size_t time;
};

class LevelManager {
public:
    LevelManager();
    static void add(Level &level) {levels.push_back(level);}

    static Level* getInstance() {
        if (instance == nullptr)
            instance = new Level();
        return instance;
    }

    void load(size_t offset = 0);
    void unload();
    void reset() { index = 0; unload(); stats.clear(); }
    static size_t get_index() { return index; }
    static void draw();
    static std::vector<levelStatistics> stats;
private:
    static Level* instance;
    static size_t index;
    static std::vector<Level> levels;
};

Level* LevelManager::instance = nullptr;
size_t LevelManager::index = 0;
std::vector<Level> LevelManager::levels;
std::vector<levelStatistics> LevelManager::stats(3);

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
    void draw();
    void run();
    int selected() {return selection;}
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

    void draw();
    int getKey();
    void increaseDecrease();
    void run();

    int getValue(int index) {
        return parameters[index].value;
    }
protected:
    std::vector<Parameters> parameters = {{KEY_W, Parameters::key},{KEY_A, Parameters::key},{KEY_S, Parameters::key}, {KEY_D, Parameters::key}, {KEY_U, Parameters::key}, {3, Parameters::speed}};
    bool selected;
    float offset = 0.5f;
};

#endif //LUNALIB_H
