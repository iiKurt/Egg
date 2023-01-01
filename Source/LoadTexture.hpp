#pragma once

#include <SDL.h>

#ifdef _WIN32
SDL_Texture* LoadTexture(SDL_Renderer* renderer, int resource);
#else
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* resource);
#endif
