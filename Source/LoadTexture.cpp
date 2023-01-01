#include "LoadTexture.hpp"

#include <SDL.h>

SDL_Texture* LoadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

#ifdef _WIN32
#include <windows.h>
#include "Resources.h"

SDL_Texture* LoadTexture(SDL_Renderer* renderer, int resource) {
	HMODULE handle = ::GetModuleHandle(NULL);
	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(resource), MAKEINTRESOURCE(FILE_BITMAP));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	DWORD size = ::SizeofResource(handle, rc);
	void* data = ::LockResource(rcData);

	SDL_RWops* io = SDL_RWFromConstMem(data, size);
	
	// 1 means to close the stream after read
	SDL_Surface* surface = SDL_LoadBMP_RW(io, 1);
	
	return LoadTextureFromSurface(renderer, surface);
	
	// TODO: cleanup memory
}
#else
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* resource) {
	return LoadTextureFromSurface(renderer, SDL_LoadBMP(resource));
}
#endif

// Generic
// Surface is freed after texture conversion
SDL_Texture* LoadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
	// The final texture
	SDL_Texture* texture = NULL;

	if (surface == NULL)
	{
		// Unable to load as a BMP
		return NULL;
	}
	else
	{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
		{
			// Unable to create texture from the surface
			return NULL;
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(surface);
	}

	return texture;
}
