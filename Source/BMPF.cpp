#include "BMPF.hpp"

#define CHARACTERS_PER_ROW    16
#define CHARACTERS_PER_COLUMN 16

static SDL_Texture* selectedFont = NULL;
static Uint16 selectedFontWidth, selectedFontHeight;

static SDL_Rect s_rect;
static SDL_Rect d_rect;

void BMPF_initalise(SDL_Texture* font)
{
	Uint32 format;
	int access;
	int w, h;

	SDL_QueryTexture(font, &format, &access, &w, &h);

	selectedFont = font;
	selectedFontWidth = w;
	selectedFontHeight = h;
	
	s_rect.w = selectedFontWidth / CHARACTERS_PER_ROW;
	s_rect.h = selectedFontHeight / CHARACTERS_PER_COLUMN;
	d_rect.w = s_rect.w;
	d_rect.h = s_rect.h;
}

void BMPF_kill(void) {
	SDL_DestroyTexture(selectedFont);
	selectedFont = NULL;
}

void BMPF_setColorFromSDL(SDL_Renderer* renderer)
{
	Uint8 r, g, b, a;
	
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetTextureColorMod(selectedFont, r, g, b);
}

void BMPF_setColor(Uint32 fore, Uint32 unused) /* Color must be in 0x00RRGGBB format! */
{
	// TODO: grab SDL renderer color instead of manually specifying?
	SDL_Color pal[1];
	pal[0].r = (Uint8)((fore & 0x00FF0000) >> 16); 
	pal[0].g = (Uint8)((fore & 0x0000FF00) >> 8);
	pal[0].b = (Uint8)((fore & 0x000000FF));
	SDL_SetTextureColorMod(selectedFont, pal[0].r, pal[0].g, pal[0].b);
}

void BMPF_Print(SDL_Renderer* renderer, const char* str, Uint32 x, Uint32 y, bool alignCenter)
{
	BMPF_setColorFromSDL(renderer);
	
	// Get screen size informations
	int rendererHeight;
	int rendererWidth;
	
	SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
	
	// Don't render off screen
	if (x > rendererWidth) {
		return;
	}
	
	// Calculate string specifications
	// Calculate the maximum characters that will fit on screen (only applies to left alignment)
	// TODO: BUG: need to +1 here to avoid cutting off characters in fallback wrapping mode... bug only occurs in left align mode
	int maxScreenCharacters = (rendererWidth - x) / (selectedFontWidth / CHARACTERS_PER_ROW) + 1;
	int breakWordIndex = -1;
	
	// Set up stuff
	int positionX = x;
	int positionY = y;

	// Draw each character, incrementing the pointer until it points to null
	for (int i = 0; str[i] != '\0'; i++)
	{
		// First line or
		// Designated line break or
		// Just passed an... unexpected newline
		if (i == 0 ||
			i == breakWordIndex ||
			str[i - 1] == '\n')
		{
			// Calculate the available space on this new line (only needed for center alignment)
			if (alignCenter) {
				// Check right side available space
				if (x >= rendererWidth / 2) {
					maxScreenCharacters = ((rendererWidth - x) * 2) / (selectedFontWidth / CHARACTERS_PER_ROW);
				}
				// Check left side available space
				else {
					maxScreenCharacters = (x * 2) / (selectedFontWidth / CHARACTERS_PER_ROW);
				}
			}
			
			// Look backwards through this line of the string, starting at the last available space on this line, stopping once we reach what has already been printed
			int endOfLineIndex = i + maxScreenCharacters;
			
			// If there is a space at the end of this line
			// Determine where to break by working through the string segment, finding either the first newline or last space character
			for (int j = i; j < endOfLineIndex; j++) {
				// End of string, don't break at all
				if (str[j] == '\0') {
					breakWordIndex = j + 1;
					break;
				}
				// Space or newline works
				else if (str[j] == ' ') {
					// Adding + 1 because we want to be able to draw end-of-line spaces off-screen and break afterwards to start fresh on a new line without a leading space
					breakWordIndex = j + 1;
				}
				else if (str[j] == '\n') {
					breakWordIndex = j + 1;
					break;
				}
			}
			// If a suitable break couldn't be found, then fallback to wrapping around wherever possible
			if (breakWordIndex == i) { // Unchanged
				// Subtracting - 1 so we don't render half a character off screen
				breakWordIndex = endOfLineIndex - 1;
			}
			
			if (alignCenter) {
				int stringLengthPixels = (breakWordIndex - 1 - i) * selectedFontWidth / CHARACTERS_PER_ROW;
				stringLengthPixels /= 2;
				
				int centeredX = x - stringLengthPixels;
				
				if (centeredX < 0) {
					centeredX = 0;
				}
				
				// Reset to the left side
				positionX = centeredX;
			}
			else {
				// Reset to the left side
				positionX = x;
			}
			
			// Not the first line
			if (i != 0) {
				// Advance to the next line
				positionY += s_rect.h;
			}
		}
		
		// Get character ASCII code
		int id = (int)str[i];
		
		// Ensure the character isn't a newline character
		// And that it will actually be drawn on screen
		if (str[i] != '\n') {
			BMPF_Print(renderer, str[i], positionX, positionY);
			
			// Advance the cursor to the left
			positionX += s_rect.w;
		}
	}
}

void BMPF_Print(SDL_Renderer* renderer, char character, Uint32 x, Uint32 y) {
	d_rect.x = x;
	d_rect.y = y;
	
	// Get character ASCII code
	int id = (int)character;
	
	// Grab character from bitmap font
	#if (CHARACTERS_PER_COLUMN != 1)
	// Get row and column coordinates
	int row = id / CHARACTERS_PER_ROW;
	int col = id % CHARACTERS_PER_ROW;
	// Convert to pixel values
	s_rect.x = col * s_rect.w;
	s_rect.y = row * s_rect.h;
	#else
	s_rect.x = id * s_rect.w;
	s_rect.y = 0;
	#endif

	// Copy the character to the screen
	SDL_RenderCopy(renderer, selectedFont, &s_rect, &d_rect);
}
