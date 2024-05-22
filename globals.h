#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "lunalib.h"

#include <string>
#include <cstddef>

bool isGameCompleted = false;
bool tutorialWASD = false, tutorialSkulls = false, tutorialBlood = false;

/* Loaded Level Data */

LevelManager levelManager;

/* Player Data */

Player player;

/* Graphics Metrics */

const float CELL_SCALE = 0.8f; // An aesthetic parameter to add some negative space around level
const float SCREEN_SCALE_DIVISOR = 700.0f; // The divisor was found through experimentation
                                           // to scale things accordingly to look pleasant.

float screenWidth;
float screenHeight;
float screenScale; // Used to scale text/UI components size and displacements based on the screen size
float cellSize;
float shiftToCenterCellByX;
float shiftToCenterCellByY;

/* Animation */

int animationFrame = 0;
class Animation {
public:
    enum animate {
        fade_out,
        fade_in,
        none
    };

    Animation(int duration) {
        animation = none;
        animationDuration = duration;
    }

    static void run();

    static animate state() {
        return animation;
    }

    static void transition(animate state) {
        animationFrame = 0;
        animation = state;
    };
private:
    static animate animation;
    static int animationDuration;
};

Animation::animate Animation::animation;
int Animation::animationDuration;

Animation animation(60);

Prompt *displayPrompt;

/* Game State */

enum gameState {
    INTRO_STATE,
    MENU_STATE,
    SELECT_LEVEL_STATE,
    OPTION_STATE,
    GAME_STATE,
    STATISTIC_STATE,
    PAUSED_STATE,
    ENDING_STATE
};

gameState gameState = INTRO_STATE;
long startTime;

/* Images and Sprites */

Texture2D playerRegular;
Texture2D playerInvert;
Texture2D owner;

Texture2D goalImage;
Texture2D boxImage;
Texture2D boxOnGoalImage;

Texture2D house1;
Texture2D house2;
Texture2D house3;
Texture2D city1;
Texture2D city2;
Texture2D city3;
Texture2D wilderness1;
Texture2D wilderness2;
Texture2D wilderness3;
Texture2D floor1;
Texture2D floor2;
Texture2D floor3;

Texture2D candleOff;
Texture2D candleOn;
Texture2D blood;

Texture2D bg;

Texture2D intro1;
Texture2D intro2;
Texture2D intro3;
Texture2D intro4;
Texture2D endingGood1;
Texture2D endingGood2;
Texture2D endingGood3;
Texture2D endingBad1;
Texture2D endingBad2;
Texture2D endingBad3;

Slideshow intro(270);
Slideshow endingGood(300);
Slideshow endingBad(300);
struct sprite {
    size_t frameCount    = 0;
    size_t framesToSkip = 3;
    size_t framesSkipped = 0;
    size_t frameIndex    = 0;
    bool loop = true;
    size_t prevGameFrame = 0;
    Texture2D *frames = nullptr;
};

/* Sounds */

Sound alert;
Sound scroll;
Sound forward;
Sound backout;
Sound reset;
Sound undo;

Sound burial1;
Sound burial2;
Sound burial3;
Sound match;

Sound hurt;

Sound levelComplete;
Sound ending;

Music theme;
Music mainTheme;
Music house;
Music town;
Music forest;

/* Forward Declarations */

// GRAPHICS_H

void deriveGraphicsMetricsFromLoadedLevel();
void tutorials();

// IMAGES_H

void loadFonts();
void unloadFonts();

void loadImages();
void unloadImages();
void drawImage(Texture2D image, float x, float y, float width, float height);
void drawImage(Texture2D image, float x, float y, float size);

sprite loadSprite(
    const std::string &fileNamePrefix,
    const std::string &fileNameSuffix,
    size_t frameCount = 1,
    bool loop = true,
    size_t framesToSkip = 3
);
void unloadSprite(sprite &sprite);
void drawSprite(sprite &sprite, float x, float y, float width, float height);
void drawSprite(sprite &sprite, float x, float y, float size);

// SOUNDS_H

void loadSounds();
void unloadSounds();
void play(Sound sound);
void play(Music music);

/* Menu Text Parameters */

Menu mainMenu({
       {"Play",         [] { Animation::transition(Animation::fade_out);}},
       {"Choose Level", [] { gameState = SELECT_LEVEL_STATE;}},
       {"Settings",     [] { gameState = OPTION_STATE;}},
       {"Exit",         [] {CloseWindow();}}
}, [] {CloseWindow();}, WHITE, GRAY, 50.0f, {0.2f, 0.4f}, {0.0f, 0.075f});
Text mainMenuTitle("Catastrophic", RED, 80.0f, {0.2f, 0.2f}, 4.0f);
Text mainMenuByline("By @lunamaltseva", GRAY, 30.0f, {0.2f, 0.85f}, 2.0f);

Menu selectLevelMenu({
   {"Level 1", [] { Animation::transition(Animation::fade_out);}},
   {"Level 2", [] { Animation::transition(Animation::fade_out);}},
   {"Level 3", [] { Animation::transition(Animation::fade_out);}}
}, [] { gameState = MENU_STATE;}, WHITE, GRAY, 50.0f, {0.37f, 0.475f}, {0.12f, 0.0f});

Text optionsMenuTitle("Settings", WHITE, 75.0f, {0.43f, 0.205f}, 4.0f);
OptionsMenu optionsMenu({
     {"Move Up", []          {optionsMenu.getKey();}},
     {"Move Left", []        {optionsMenu.getKey();}},
     {"Move Down", []        {optionsMenu.getKey();}},
     {"Move Right", []       {optionsMenu.getKey();}},
     {"Undo Move", []        {optionsMenu.getKey();}},
     {"Tiles per Second", [] {optionsMenu.increaseDecrease();}}
}, [] { gameState = MENU_STATE;}, WHITE, GRAY, 40.0f, {0.4f, 0.4f}, {0.0f, 0.05f});

MultilineText pauseMenuTitle("Bear It Out\neven to the\nEdge of Doom", {0.0f, 0.075f}, RED, 70.0f, {0.5f, 0.2f});
Menu pauseMenu({
       {"Continue",  [] { gameState = GAME_STATE;}},
       {"Restart",   [] {Animation::transition(Animation::fade_out); play(reset);}},
       {"Main menu", [] {Animation::transition(Animation::fade_out); }}
}, [] { gameState = GAME_STATE;}, WHITE, GRAY, 50.0f, {0.5f, 0.6f}, {0.0f, 0.075f});

Menu levelCompletedMenu({
     {"Continue",  [] { Animation::transition(Animation::fade_out);}},
     {"Restart",   [] { Animation::transition(Animation::fade_out);}},
}, [] { ; }, WHITE, GRAY, 50.0f, {0.5f, 0.65f});

#endif // GLOBALS_H
