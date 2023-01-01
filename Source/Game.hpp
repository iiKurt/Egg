#pragma once

#include <SDL.h>

const int GameArrayHeight = 2;
const int GameArrayLength = 8;
int GameArray[GameArrayHeight][GameArrayLength] = {{ 0 }};

static const int StartIndex = GameArrayLength - 1;
int GameCurrentIndex = StartIndex;

void Game_Initalise();
char Game_GetHexCharacter(int n);
