#include "DrawPSF.hpp"

#include "Font/LoadPSF.hpp"
#include <SDL.h>

#include <iostream>

// TODO:
// Draw pixels to an SDL_Texture in DrawPSF.cpp, return the texture?
// Scale the texture or whatever then RenderCopy or somehow get the renderer to draw the texture?

void PrintCharacter(SDL_Renderer* renderer, PSF1_FONT* font, char chr, unsigned int xOff, unsigned int yOff) {
	// Multiply character we want to print by the height of the character
	// Character we want can be found by adding offset onto the glyphBuffer starting point
	char* fontPtr = (char*)font->glyphBuffer + (chr * (int)font->psf1_Header->charsize); // Indexing

	for (unsigned long y = yOff; y < yOff + 16; y++) { // 16 bits high
		for (unsigned long x = xOff; x < xOff + 8; x++) { // 8 bits long, and is set to 1
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) { // Select the bit that we need
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
		fontPtr++;
	}
}

void PrintString(SDL_Renderer* renderer, PSF1_FONT* font, const char* str, unsigned int x, unsigned int y) {
	char* chr = (char*)str;

	while (*chr != 0) { // Null terminated string
		PrintCharacter(renderer, font, *chr, x, y);

		x += 8; // Move 8 pixels to the right
		chr++;
	}
}
