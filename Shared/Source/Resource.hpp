#pragma once

#include <SDL2/SDL.h>

class Resource
{
private:
	char* buffer;
public:
	const char* path;
	SDL_RWops* io;

	Resource(const char* _path);
	Resource(const char* _path, int name, int type);
	~Resource();
};
