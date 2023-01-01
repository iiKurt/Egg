#pragma once

#include <SDL.h>

void BMPF_initalise(SDL_Texture* font);
void BMPF_kill(void);
void BMPF_setColorFromSDL(SDL_Renderer* renderer);
void BMPF_setColor(Uint32 color, Uint32 unused); /* Color must be in 0x00RRGGBB format! */
void BMPF_Print(SDL_Renderer* renderer, const char* str, Uint32 x, Uint32 y, bool alignCenter);
void BMPF_Print(SDL_Renderer* renderer, char character, Uint32 x, Uint32 y);
