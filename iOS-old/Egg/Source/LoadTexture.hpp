#pragma once

#include <SDL.h>
#include <exception>

#include "Resource.hpp"
#include "File.hpp"

SDL_Texture* LoadTexture(SDL_Renderer* renderer, Resource* io);

// TODO: split into multiple classes based on a create texture error and load texture error?
class InvalidTextureError : public FileError
{
private:
	const char* SDLError;
public:
	InvalidTextureError(const char* _path, const char* _SDLError);
	virtual const char* what() const noexcept;
};
