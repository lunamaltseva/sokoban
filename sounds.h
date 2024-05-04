#ifndef SOUNDS_H
#define SOUNDS_H

#include "raylib.h"

#include "globals.h"

void load_sounds() {
    InitAudioDevice();
    scroll = LoadSound("data/sounds/scroll.ogg");
    forward = LoadSound("data/sounds/click.ogg");
    backout = LoadSound("data/sounds/backout.ogg");
}

void unload_sounds() {
    CloseAudioDevice();
}

void play_sound(Sound sound) {
    PlaySound(sound);
}

#endif // SOUNDS_H
