#pragma once

#include <SDL.h>

#include "../File.hpp"
#include "../Resource.hpp"

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

// There really doesn't need to be a whole class hierachy here (we don't need FontError)... could just be InvalidFontError...
class FontError : public FileError // Is an abstract class
{
public:
	FontError(const char* _path);
	// Pure virtual function (= 0), is const will be terminated if throws an exception
	virtual const char* what() const noexcept = 0;
};

class InvalidFontError : public FontError
{
public:
	InvalidFontError(const char* _path);
	virtual const char* what() const noexcept;
};

PSF1_FONT* LoadPSF1Font(Resource* resource);
