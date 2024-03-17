#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#include <string>
#include <cstddef>
#include <vector>

/* Game Elements */

const char FLOOR     = ' ';
const char WALL      = '#';
const char ENTRANCE  = 'P';
const char EXIT      = 'E';
const char KEY       = '*';
const char MARBLE    = '.';
const char QUICKTILE = '!';
const char PAWN      = '1'; // Typical chess values for pieces
const char ROOK      = '5';
const char QUEEN     = '9';

enum gameElements {
    keyElement = 0, marbleElement = 1, quicktileElement = 2, pawnElement = 3, rookElement = 4, queenElement = 5
};
const int gameElementsCount = 6;
bool gameElements[gameElementsCount]{false};

/* Levels */

class level {
public:
    size_t rows = 0, columns = 0, moves = 0, keys = 0;
    char *data = nullptr;
};

// Level 1

char LEVEL_1_DATA[] = {
        '#', '#', '#', '#', '#', '#', '#', '#', '#',
        '#', ' ', ' ', '*', ' ', ' ', ' ', ' ', '#',
        '#', ' ', '#', '#', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', 'P', ' ', '#', ' ', '#',
        '#', ' ', ' ', ' ', 'E', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', '#', '#', ' ', '#', '#', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', '*', ' ', '#',
        '#', '#', '#', '#', '#', '#', '#', '#', '#',
};

level LEVEL_1 = {
        9, 9, 23, 2,
        LEVEL_1_DATA
};

// Level 2

char LEVEL_2_DATA[] = {
        '#', '#', '#', '#', '#', '#', '#', '#', '#',
        '#', 'P', ' ', ' ', ' ', ' ', '#', 'E', '#',
        '#', ' ', '#', '#', '#', ' ', ' ', ' ', '#',
        '#', ' ', '#', '*', '#', '1', '#', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', '#', ' ', '#', '#', ' ', ' ', '#',
        '#', ' ', ' ', ' ', '#', '#', ' ', '#', '#',
        '#', ' ', ' ', ' ', ' ', ' ', '*', ' ', '#',
        '#', '#', '#', '#', '#', '#', '#', '#', '#'
};

level LEVEL_2 = {
        9, 9, 24, 2,
        LEVEL_2_DATA
};

// Level 3

char LEVEL_3_DATA[] = {
        '#','#','#','#','#','#','#','#','#',
        '#',' ',' ','E','#',' ',' ','#','#',
        '#','P',' ',' ','.',' ','*',' ','#',
        '#',' ','#',' ','#',' ',' ',' ','#',
        '#',' ',' ',' ',' ',' ','#','#','#',
        '#','#','#',' ','#',' ',' ',' ','#',
        '#','#',' ',' ',' ',' ',' ','#','#',
        '#','#','*',' ','#',' ','.','5','#',
        '#','#','#','#','#','#','#','#','#',
};

level LEVEL_3 = {
        9, 9, 21, 2,
        LEVEL_3_DATA
};

// Level 4

char LEVEL_4_DATA[] = {
        '#', '#', '#', '#', '#', '#', '#', '#', '#',
        '#', ' ', ' ', 'P', ' ', ' ', ' ', '1', '#',
        '#', '!', '#', '!', '#', '!', '#', '!', '#',
        '#', ' ', '!', 'E', '!', ' ', '#', '*', '#',
        '#', ' ', '#', '!', '#', '#', '#', ' ', '#',
        '#', ' ', ' ', ' ', '!', ' ', ' ', ' ', '#',
        '#', '!', '#', '!', '#', '!', '#', '!', '#',
        '#', ' ', ' ', ' ', ' ', ' ', '!', ' ', '#',
        '#', ' ', '*', '#', '#', '.', ' ', '5', '#',
        '#', '#', '#', '#', '#', '#', '#', '#', '#',
};

level LEVEL_4 = {
        10, 9, 23, 2,
        LEVEL_4_DATA
};

// Level 5

char LEVEL_5_DATA[] = {
        '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
        '#', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', '#', '!', '#', '!', '#', ' ', '#', ' ', '#',
        '#', '*', '!', 'P', ' ', ' ', '!', ' ', '!', ' ', '#',
        '#', ' ', '#', '!', '#', ' ', '#', '!', '#', ' ', '#',
        '#', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', '#',
        '#', '!', '#', '.', '#', ' ', '#', '!', '#', ' ', '#',
        '#', ' ', ' ', ' ', '!', ' ', ' ', '9', '*', ' ', '#',
        '#', ' ', '#', '!', '#', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
};

level LEVEL_5 = {
        11, 11, 31, 3,
        LEVEL_5_DATA
};

// All together

const size_t LEVEL_COUNT = 5;
level LEVELS[LEVEL_COUNT] = {
    LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_4, LEVEL_5
};

// Loaded Level Data

bool isExitOpen;
char* loadLevel = nullptr;
level level;
size_t startFromLevel = 0;
size_t levelIndex = startFromLevel;

/* Figures */

class Figure {
public:
    size_t row, column; Texture2D image;                                                                  // crucial
    int position; char cell; Vector2 location;                                                            // optimization
    bool isFalling = false, isToBeKilled = false, isKilled = false, shouldJump = false;                   // cosmetic
};

class Player : public Figure {
public:
    int keys = 0, livesMax = 3, lives = 3; // game mechanics
};

// Create figures

Player player;

// Figure out who moves after whom
enum moveSequence {NONE, DONE, WAIT};
moveSequence moveSequence[4];

/* Animation */

enum playAnimation {
    fromMenu,           // fade when loading a level
    playerMoving,       // smooth figure movement
    playerFalling,      // figure aboard!
    playerKilled,       // if player is killed
    playerOutOfMoves,   // if the moves-clock ran out
    levelStarting,      // smooth fade-in and fade-out when loading the level
    levelEnding,
    gameOver,           // print the game over message
};
playAnimation playAnimation;

int imageIteration = 0;
int animationDuration = 120;
int framesPerEntity;
Vector2 positionFrom;
Vector2 positionTo;
Vector2 path;
size_t beginningFrame = 0;

/* Graphics Metrics */

const float CELL_SCALE = 0.85f; // An aesthetic parameter to add some negative space around level
const float screenScale_DIVISOR = 85.3f; // The divisor was found through experimentation
                                          // to scale things accordingly to look pleasant.
float screenWidth;
float screenHeight;
float screenScale; // Used to scale text/UI components size and displacements based on the screen size
float cellSize;
float shiftToCenterCellByX;
float shiftToCenterCellByY;

/* Assets */

// =-=-=-=-= Fonts =-=-=-=-=
Font menuFont;

// =-=-=-=-= Images and sprites =-=-=-=-=
struct sprite {
    size_t frameCount    = 0;
    size_t framesToSkip  = 3;
    size_t framesSkipped = 0;
    size_t frameIndex    = 0;
    bool loop = true;
    size_t prevGameFrame = 0;
    Texture2D *frames = nullptr;
};

// Story
Texture2D story0;
Texture2D story1;
Texture2D story2;
Texture2D story3;
Texture2D story4;
Texture2D story5;

// Menus
Texture2D menuTitle;
sprite tutorialSprite;

// Levels
Texture2D wallDepthWhiteImage;
Texture2D wallDepthBlackImage;
Texture2D wallDepthQuickTileImage;
Texture2D wallNoneImage;
Texture2D boardWhiteImage;
Texture2D boardBlackImage;
Texture2D boardQuickTileImage;
Texture2D boardHaloImage;
Texture2D entranceImage;
Texture2D exitImage;
Texture2D exitClosedImage;

// Elements
Texture2D keyImage;
Texture2D keyOutlineImage;
Texture2D quickTileImage;
sprite marbleSprite;
sprite keySprite;

// Figures
Texture2D playerImage;

// =-=-=-=-= Sounds and Music =-=-=-=-=

// GUI
Sound menuExitSound;
Sound clickSound;
Sound scrollSound;
Sound backOutSound;

// Clock
Sound clockTickSound0;
Sound clockTickSound1;
Sound clockHighSound;
Sound clockNormalSound;
Sound clockLowSound;

// Figures
Sound playerLandedSound;
Sound figureFallSound;

// Level & Elements;
Sound puzzleCompleteSound;
Sound exitOpenSound;
Sound pickupKeySound;
Sound marbleTopupSound;
Sound quicktileBreakSound0;
Sound quicktileBreakSound1;
Sound levelBreakSound;

// Music
Music mainTheme;
Music introTheme;
Music endingTheme;

/* Frame Counter */

size_t gameFrame = 0;

/* Game State */

enum gameState {
    INTRO_STATE,
    MENU_STATE,
    CHOOSE_LEVEL_STATE,
    TUTORIAL_STATE,
    GAME_STATE,
    ANIMATION_STATE,
    PAUSED_STATE,
    VICTORY_STATE,
    GAME_OVER_STATE
};

gameState gameState = INTRO_STATE;

/* Text and Menus */

class Text {
public:
    std::string line;
    const float size, xShift, yShift;
    const Color color;
};

class AnimatedText : public Text {
public:
    std::vector<std::string> entries;
    const int frametime;
    int textIndex = 0;
};

class Menu {
public:
    std::vector<std::string> entries;
    const float size, xShift, yShift, dX, dY;
    const Color selected, unselected;
    int entrySelected;
    enum menuType {MAIN_MENU_TYPE = 0, CHOOSE_LEVEL_TYPE = 1, PAUSE_TYPE = 2, GAME_OVER_TYPE = 3, VICTORY_TYPE = 4}; menuType type;
};

// Intro
Text introHint = {"Press W or ^ to move forward", 30.0f, 0.0f, 300.0f, WHITE};

// Title
const float MENU_TITLE_SIZE = 400.0f;
Menu mainMenu = {{"Play", "Load Level", "Tutorial", "Quit"},
                 30.0f, 0.0f, 50.0f, 0.0f, 50.0f,
                 {255, 255, 255, 255}, {100, 100, 100, 255},
                 0, Menu::MAIN_MENU_TYPE};
Text authorship = {"by @lunamaltseva", 15.0f, 0.0f, 300.0f, {100, 100, 100, 255}};

// Choose Level
Menu chooseLevelMenu = {{"Level 1", "Level 2", "Level 3", "Level 4", "Level 5"},
                        30.0f, -300.0f, 100.0f, 150.0f, 0.0f,
                        {255, 255, 255, 255}, {100, 100, 100, 255},
                        0, Menu::CHOOSE_LEVEL_TYPE};

// Pause
Menu pauseMenu = {{"Resume", "Restart Level", "Exit to Main Menu", "Quit Game"},
                  30.0f, 0.0f, -50.0f, 0.0f, 50.0f,
                  {255, 255, 255, 255}, {100, 100, 100, 255},
                  0, Menu::PAUSE_TYPE};
bool exitFromAnimation;

// Game Over
AnimatedText gameOverTitle = {"This is not how it ends.", 60.0f, 0.0f, -100.0f, RED, {"This", "This is", "This is not", "This is not how", "This is not how it"}, 25};
Menu gameOverMenu = {{"Try again", "Exit to Main Menu", "Quit Game"},
                    30.0f, 0.0f, 50.0f, 0.0f, 50.0f,
                    {255, 255, 255, 255}, {100, 100, 100, 255},
                    0, Menu::GAME_OVER_TYPE};

// Victory
Text victoryTitle = {"FINIS", 60.0f, 0.0f, -100.0f, WHITE};
Menu victoryMenu = {{"Exit to Main Menu", "Quit Game"},
                     30.0f, 0.0f, 50.0f, 0.0f, 50.0f,
                     {255, 255, 255, 255}, {100, 100, 100, 255},
                     0, Menu::VICTORY_TYPE};

/* =-=-=-=-=-=-=-=-= Forward Declarations =-=-=-=-=-=-=-=-= */

/* LEVELS_H */

// Without loading any level, don't break the menu
void initGraphics();

// Pre-generation
void initLevel();
void placePlayer();

// Generate
void createLevelInstance();
void loadNextLevel(int jump);

// Complete animation
void completeLevelDrawing();

// Misc
void listLevelElements(bool array[], size_t size, size_t checkLevelIndex);
void restartLevel();

/* FIGURE_H */

// Misc
float simplifyNumber(float num);

// ==-- Player --==
void movePlayer(int dx, int dy);
void playerCompleteMovement();


// DRAWING_H

// Text utilities
void drawText(const Text &text, float size, Vector2 position);
void drawText(const Text &text);
void drawAnimatedText(AnimatedText &text);
void drawIconsAboveText(const Text &text, size_t checkLevelIndex);
void drawMenu(const Menu &menu);

// Image utilities
void drawImage(Texture2D image, float x, float y, float size);
void drawImage(Texture2D image, float x, float y, float width, float height);
void drawSprite(sprite &sprite, float x, float y, float size);
void drawSprite(sprite &sprite, float x, float y, float width, float height);

// Level utilities
void deriveGraphicsMetricsFromLoadedLevel();
void drawLevelHalo(int i);

Texture2D floorTexture(size_t row, size_t column);
Texture2D wallTexture(size_t row, size_t column);
void drawLevel();

// Figure utilities
void drawFigure(Figure figure);
void drawFigures();

// Fade-in utilities
void fade(unsigned char transparency);
void fade(int i, float step);
void fade(int i, float step, float baseline);
void fade(int i, int from, int until, unsigned char transparency);
void fade(int i, int from, int until, float step, float baseline);

/* ANIMATION_H */

// Specifically for figures
void initMovement(Figure figure, size_t rowFrom, size_t columnFrom, size_t rowTo, size_t columnTo);

// Set up for animation
void initAnimation();

// Simplify animation
void animateMovement(Figure &figure, int i);
void animateFalling(Figure &figure, int i);

// Finish animation and ...
void endAnimation(int i);

// Animate
void drawAnimation();

/* UI_UX_H */

// Enable changing selected items in menus
void controlMenu(Menu &menu);

// Shorthand
void toMainMenu();
void fromMainMenu();

// Simplify Input
bool isAffirmativeButtonPressed();
bool isNegativeButtonPressed();

// Simplify updateGame()
void controlMusic();
void controlEsc();
void controlMenus();
void interpretSelection(Menu &menu);

// In-game
void drawGUI();

// Draw states
void drawIntro();
void drawMainMenu();
void drawChooseLevelMenu();
void drawTutorial();
void drawPauseMenu();
void drawGameOverMenu();
void drawVictoryMenu();

/* ASSETS_H */
// Fonts
void loadFonts();
void unloadFonts();

// Images & sprites
void loadImages();
sprite loadSprite(const std::string &file_name_prefix, const std::string &file_name_suffix, size_t frameCount = 1, bool loop = true, size_t framesToSkip = 30);
void unloadImages();
void unloadSprite(sprite &sprite);

// Sounds
void loadSounds();
void unloadSounds();

#endif // GLOBALS_H
