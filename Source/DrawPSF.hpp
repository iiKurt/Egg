#pragma once

#include "Font/LoadPSF.hpp"
#include <SDL.h>

void PrintCharacter(SDL_Renderer* renderer, PSF1_FONT* font, char chr, unsigned int xOff, unsigned int yOff);
void PrintString(SDL_Renderer* renderer, PSF1_FONT* font, const char* str, unsigned int x, unsigned int y);
