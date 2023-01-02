#pragma once

#include <SDL.h>

void BMPF_Initalise(SDL_Texture* font);
void BMPF_Kill(void);
void BMPF_SetColorFromSDL(SDL_Renderer* renderer);
void BMPF_SetColor(Uint8 r, Uint8 g, Uint8 b);
void BMPF_SetColor(Uint32 color, Uint32 unused); /* Color must be in 0x00RRGGBB format! */
void BMPF_Print(SDL_Renderer* renderer, const char* str, Uint32 x, Uint32 y, bool alignCenter);
void BMPF_Print(SDL_Renderer* renderer, char character, Uint32 x, Uint32 y);
