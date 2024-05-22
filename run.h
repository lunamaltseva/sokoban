#ifndef RUN_H
#define RUN_H

#include "globals.h"
#include "lunalib.h"

void playLevelMusic() {
    switch(levelManager.get_index()) {
        case 0: play(house); break;
        case 1: play(town); break;
        case 2: play(forest); break;
    }
}

Level LevelDecoder::instantiate(std::string &line) {
    expand(line);
    std::vector<std::string> lines;
    fillOut(lines, line);

    size_t columns = lines[0].length();
    for (auto &el : lines) if (el.length() != columns)
            displayPrompt = new Prompt("Error Loading Level!", std::string("One of the levels has corrupted data."));

    size_t rows = lines.size();

    char* data;
    data = new char[rows*columns];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            data[i * columns + j] = lines[i].at(j);
        }
    }
    return {rows, columns, data};
}

void LevelDecoder::expand(std::string &str) {
    std::string result;
    for (int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i]))
            result += str[i];

        else {
            std::string temp;
            int index = 0;
            while (isdigit(str[i + index]))
                temp += str[i + index++];

            char c = str[i += index];
            int value = stoi(temp);
            while (value--> 0)
                result+=c;
        }
    }

    str = result;
}

std::vector<std::string> LevelDecoder::fillOut(std::vector<std::string>& vector, const std::string &str) {
    std::stringstream stream(str);
    while (!stream.eof()) {
        std::string token;
        std::getline(stream, token, '|');
        vector.push_back(token);
    }

    size_t max_length = 0;
    for (auto &el : vector)
        if (max_length<el.length())
            max_length=el.length();
    for (auto &el : vector)
        while (el.length()!=max_length)
            el.append(" ");

    return vector;
};

LevelManager::LevelManager()  {
    std::ifstream input("data/levels.sl");
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        if (line[0] == ';') continue;
        Level loaded_level = LevelDecoder::instantiate(line);
        add(loaded_level);
    }
    stats[0].steps = stats[1].steps = stats[2].steps = 0;
}

void Menu::run() {
        if (mv_down())         {if (entry.size() <= ++selection) selection = 0; PlaySound(scroll);}
        else if (mv_up())      {if (0 > --selection) selection = entry.size()-1;PlaySound(scroll);}
        else if (mv_forward()) {entry[selection].forward();PlaySound(forward);}
        else if (mv_back())    {backward();PlaySound(backout);}
        this->draw();
}

int OptionsMenu::getKey() {
    int stroke = GetKeyPressed();
    if (stroke != 0) {
        parameters[selection].value = stroke;
    }
    return parameters[selection].value;
}

void OptionsMenu::increaseDecrease() {
    int &val = parameters[selection].value;

    if (val>=1 && val<=60) {
        if (mv_down() && val!=60) val++;
        else if (mv_up() && val!=1) val--;
    }
}

void OptionsMenu::run() {
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

void tutorials() {
    if (!levelManager.get_index()) {
        switch (levelManager.stats[0].steps) {
            case 8:
                if (!tutorialSkulls) {
                    displayPrompt = new Prompt("Tutorial [2/3]", std::string(
                            "To complete the level,\npush all skulls into graves.\n\nYou can only push 1 skull at once.\n\nPress ") +
                                                                 optionsMenu.getChar(5) +
                                                                 " to undo accidental movements.");
                    play(alert);
                }
                tutorialSkulls=true;
                break;
            case 100:
                if (!tutorialBlood) {
                    displayPrompt = new Prompt("Tutorial [3/3]", std::string(
                            "Physical exertion causes blood loss.\nEvery 100 steps, you lose blood.\n\nTry not to lose too much blood."));
                    play(alert);
                }
                tutorialBlood=true;
                break;
        }
    }
}

#endif //RUN_H
