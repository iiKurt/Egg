#include "LoadTexture.hpp"

#include <SDL.h>
#include <string>

#include "Resource.hpp"

InvalidTextureError::InvalidTextureError(const char* _path, const char* _SDLError) : FileError(_path) {
	SDLError = _SDLError;
}

const char* InvalidTextureError::what() const noexcept {
	return (std::string() + "Unable to load image/create texture, perhaps it's invalid? SDL Error: " + this->SDLError).c_str();
}

// Generic
SDL_Texture* LoadTexture(SDL_Renderer* renderer, Resource* resource)
{
	// The final texture
	SDL_Texture* newTexture = NULL;

	// Load image from abstract RWops
	SDL_Surface* loadedSurface = SDL_LoadBMP_RW(resource->io, 1);

	if (loadedSurface == NULL)
	{
		// Unable to load as a BMP
		throw InvalidTextureError(resource->path, SDL_GetError());
	}
	else
	{
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			// Unable to create texture from the surface
			throw InvalidTextureError(resource->path, SDL_GetError());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
