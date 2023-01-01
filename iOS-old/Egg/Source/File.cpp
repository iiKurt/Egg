#include "File.hpp"

#include <string>

FileError::FileError(const char* _path) {
	this->path = _path;
}

FileNotFoundError::FileNotFoundError(const char* _path) : FileError(_path) { }

#include <SDL.h>

const char* FileNotFoundError::what() const noexcept {
	// Seems like using std::string here seems to crash the program...
	// Works outside of this class however - strange!
	// Even the LoadTexture class can use it...
	return "Unable to locate requested file.";
}
