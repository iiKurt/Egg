#include "LoadPSF.hpp"

#include <SDL.h>

#include "../Resource.hpp"

FontError::FontError(const char* _path) : FileError(_path) { }
InvalidFontError::InvalidFontError(const char* _path) : FontError(_path) { }

const char* InvalidFontError::what() const noexcept
{
	return "Invalid font file.";
}

// Generic
PSF1_FONT* LoadPSF1Font(Resource* resource) {
	PSF1_HEADER* fontHeader = new PSF1_HEADER;
	// Read file into header struct from abstract RWops
	SDL_RWread(resource->io, (char*)fontHeader, sizeof(fontHeader), 1);

	// Verify that the file is actually a PSF1 font
	if (fontHeader->magic[0] != PSF1_MAGIC0 ||
		fontHeader->magic[1] != PSF1_MAGIC1) { // It's not a valid font
		throw InvalidFontError(resource->path);
	}

	unsigned int glyphBufferSize = fontHeader->charsize * 256; // number of glyphs (256), although it only seems to load 128?!
	if (fontHeader->mode == 1) { // there are 512 glyphs
		glyphBufferSize = fontHeader->charsize * 512;
	}

	// TODO: Using C-style memory allocation??? why...
	void* glyphBuffer = malloc(glyphBufferSize);
	{
		// Jump to the end of PSF1_HEADER in the PSF1_FONT struct
		SDL_RWseek(resource->io, sizeof(PSF1_HEADER), RW_SEEK_SET);
		//fs.seekg(sizeof(PSF1_HEADER), std::ios::beg);
		// Read file into glyphBuffer
		SDL_RWread(resource->io, (char*)glyphBuffer, glyphBufferSize, 1);
		//fs.read((char*)glyphBuffer, glyphBufferSize);
	}

	PSF1_FONT* font = new PSF1_FONT;
	font->psf1_Header = fontHeader;
	font->glyphBuffer = glyphBuffer;

	return font;
}
