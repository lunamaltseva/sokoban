#ifndef IMAGES_H
#define IMAGES_H

#include "raylib.h"

#include "globals.h"

#include <string>
#include <cassert>

void loadFonts() {
    menu_font = LoadFontEx("data/fonts/alagard.ttf", 256, 0, 128);
}

void unloadFonts() {
    UnloadFont(menu_font);
}

void loadImages() {
    playerRegular       = LoadTexture("data/images/player.png");
    playerInvert        = LoadTexture("data/images/player_inv.png");
    owner               = LoadTexture("data/images/owner.png");

    goalImage           = LoadTexture("data/images/goal.png");
    boxImage            = LoadTexture("data/images/box.png");
    boxOnGoalImage      = LoadTexture("data/images/box-on-goal.png");

    floor1              = LoadTexture("data/images/floor/floor.png");
    floor2              = LoadTexture("data/images/floor/grass.png");
    floor3              = LoadTexture("data/images/floor/tiles.png");
    house1              = LoadTexture("data/images/wall/house1.png");
    house2              = LoadTexture("data/images/wall/house2.png");
    house3              = LoadTexture("data/images/wall/house3.png");
    city1               = LoadTexture("data/images/wall/town1.png");
    city2               = LoadTexture("data/images/wall/town2.png");
    city3               = LoadTexture("data/images/wall/town3.png");
    wilderness1         = LoadTexture("data/images/wall/wilderness1.png");
    wilderness2         = LoadTexture("data/images/wall/wilderness2.png");
    wilderness3         = LoadTexture("data/images/wall/wilderness3.png");

    candleOff           = LoadTexture("data/images/candle_off.png");
    candleOn            = LoadTexture("data/images/candle_on.png");
    blood               = LoadTexture("data/images/blood.png");

    bg                  = LoadTexture("data/images/bg.png");

    intro1              = LoadTexture("data/images/slides/intro1.png");
    intro2              = LoadTexture("data/images/slides/intro2.png");
    intro3              = LoadTexture("data/images/slides/intro3.png");
    intro4              = LoadTexture("data/images/slides/intro4.png");
    endingGood1         = LoadTexture("data/images/slides/final_good1.png");
    endingGood2         = LoadTexture("data/images/slides/final_good2.png");
    endingGood3         = LoadTexture("data/images/slides/final_good3.png");
    endingBad1          = LoadTexture("data/images/slides/final_bad1.png");
    endingBad2          = LoadTexture("data/images/slides/final_bad2.png");
    endingBad3          = LoadTexture("data/images/slides/final_bad3.png");

    intro.add({"I always was a happy cat...",                     intro1});
    intro.add({"...helping my owner do his undertaker's job.",    intro2});
    intro.add({"Until... They died.",                             intro3});
    intro.add( {"They all died.",                                 intro4});

    endingGood.add({"Oh, how I miss you...",                           endingGood1});
    endingGood.add({"God, how it hurts!",                              endingGood2});
    endingGood.add({"\"Here's looking at you, kitten.\"",              endingGood3});

    endingBad.add( {"I can't live like this! I can't!..",               endingBad1});
    endingBad.add( {"I can't...",                                       endingBad2});
    endingBad.add( {"\"Sigh. What am I going to do with you, kitten.\"",endingBad3});

    player.setImage(playerRegular);
}

void unloadImages() {
    UnloadTexture(playerRegular);
    UnloadTexture(playerInvert);
    UnloadTexture(owner);
    UnloadTexture(goalImage);
    UnloadTexture(boxImage);
    UnloadTexture(boxOnGoalImage);
    UnloadTexture(floor1);
    UnloadTexture(floor2);
    UnloadTexture(floor3);
    UnloadTexture(house1);
    UnloadTexture(house2);
    UnloadTexture(house3);
    UnloadTexture(city1);
    UnloadTexture(city2);
    UnloadTexture(city3);
    UnloadTexture(wilderness1);
    UnloadTexture(wilderness2);
    UnloadTexture(wilderness3);
    UnloadTexture(candleOff);
    UnloadTexture(candleOn);
    UnloadTexture(blood);
    UnloadTexture(intro1);
    UnloadTexture(intro2);
    UnloadTexture(intro3);
    UnloadTexture(intro4);
    UnloadTexture(endingGood1);
    UnloadTexture(endingGood2);
    UnloadTexture(endingGood3);
    UnloadTexture(endingBad1);
    UnloadTexture(endingBad2);
    UnloadTexture(endingBad3);
}

void load_sounds() {
    InitAudioDevice();
    alert          = LoadSound("data/sounds/alert.ogg");
    scroll         = LoadSound("data/sounds/scroll.ogg");
    forward        = LoadSound("data/sounds/click.ogg");
    backout        = LoadSound("data/sounds/backout.ogg");
    reset          = LoadSound("data/sounds/reset.ogg");
    undo           = LoadSound("data/sounds/rewind.ogg");

    burial1        = LoadSound("data/sounds/burial1.ogg");
    burial2        = LoadSound("data/sounds/burial2.ogg");
    burial3        = LoadSound("data/sounds/burial3.ogg");
    match          = LoadSound("data/sounds/match.ogg");

    hurt           = LoadSound("data/sounds/hurt.ogg");

    levelComplete  = LoadSound("data/sounds/level_complete.ogg");
    ending         = LoadSound("data/sounds/ending.ogg");

    theme          = LoadMusicStream("data/music/theme.ogg");
    mainTheme      = LoadMusicStream("data/music/main.ogg");
    house          = LoadMusicStream("data/music/house.ogg");
    town           = LoadMusicStream("data/music/town.ogg");
    forest         = LoadMusicStream("data/music/forest.ogg");
}

void unload_sounds() {
    UnloadSound(scroll);
    UnloadSound(forward);
    UnloadSound(backout);
    UnloadSound(reset);
    UnloadSound(undo);
    UnloadSound(burial1);
    UnloadSound(burial2);
    UnloadSound(burial3);
    UnloadSound(match);
    UnloadSound(hurt);
    UnloadSound(levelComplete);
    UnloadSound(ending);
    UnloadMusicStream(theme);
    UnloadMusicStream(mainTheme);
    UnloadMusicStream(house);
    UnloadMusicStream(town);
    UnloadMusicStream(forest);
    CloseAudioDevice();
}

void play(Sound sound) {
    PlaySound(sound);
}

void play(Music music) {
    PlayMusicStream(music);
    UpdateMusicStream(music);
}

void drawImage(Texture2D image, float x, float y, float size) {
    drawImage(image, x, y, size, size);
}

void drawImage(Texture2D image, float x, float y, float width, float height) {
    Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    Rectangle destination = { x, y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

sprite loadSprite(
    const std::string &fileNamePrefix,
    const std::string &fileNameSuffix,
    size_t frameCount,
    bool loop,
    size_t framesToSkip
) {
    assert(frameCount < 100);

    sprite result = {
            frameCount, framesToSkip, 0, 0, loop, 0, new Texture2D[frameCount]
    };

    for (size_t i = 0; i < frameCount; ++i) {
        std::string file_name;
        if (frameCount < 10) {
            file_name = fileNamePrefix;
            file_name += std::to_string(i);
            file_name += fileNameSuffix;
        } else {
            file_name = fileNamePrefix;
            file_name += i < 10 ? ("0" + std::to_string(i)) : std::to_string(i);
            file_name += fileNameSuffix;
        }
        result.frames[i] = LoadTexture(file_name.c_str());
    }

    return result;
}

void unloadSprite(sprite &sprite) {
    assert(sprite.frames != nullptr);

    for (size_t i = 0; i < sprite.frameCount; ++i) {
        UnloadTexture(sprite.frames[i]);
    }
    delete[] sprite.frames;
    sprite.frames = nullptr;
}

void drawSprite(sprite &sprite, float x, float y, float size) {
    drawSprite(sprite, x, y, size, size);
}

void drawSprite(sprite &sprite, float x, float y, float width, float height) {
    drawImage(sprite.frames[sprite.frameIndex], x, y, width, height);

    if (sprite.prevGameFrame == game_frame) {
        return;
    }
    if (sprite.framesSkipped < sprite.framesToSkip) {
        ++sprite.framesSkipped;
    } else {
        sprite.framesSkipped = 0;

        ++sprite.frameIndex;
        if (sprite.frameIndex >= sprite.frameCount) {
            sprite.frameIndex = sprite.loop ? 0 : sprite.frameCount - 1;
        }
    }
    sprite.prevGameFrame = game_frame;
}

#endif // IMAGES_H
