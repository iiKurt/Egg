#include <iostream>
#include <fstream>

// PSF File loading
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

int main(int argc, char* args[])
{
	std::ifstream fs;
	fs.open("zap-light16.psf", std::ios::binary);

	if (fs.fail()) {
		std::cout << "Font not found" << std::endl;
		return 1;
	}

	PSF1_HEADER* fontHeader = new PSF1_HEADER;
	uint size = sizeof(PSF1_HEADER);

	// Read file into header struct
	fs.read((char*)fontHeader, sizeof(fontHeader));

	// Verify that the file is actually a PSF1 font
	if (fontHeader->magic[0] != PSF1_MAGIC0 ||
		fontHeader->magic[1] != PSF1_MAGIC1) { // It's not a valid font
		std::cout << "Invalid font file" << std::endl;
		return 1;
	}

	uint glyphBufferSize = fontHeader->charsize * 256; // number of glyphs (256)
	if (fontHeader->mode == 1) { // there are 512 glyphs
		glyphBufferSize = fontHeader->charsize * 512;
	}

	// C-style memory allocation go brrrrr
	void* glyphBuffer = malloc(glyphBufferSize);
	{
		// Jump to the end of PSF1_HEADER in the PSF1_FONT struct
		fs.seekg(sizeof(PSF1_HEADER), std::ios::beg);
		// Read file into glyphBuffer
		fs.read((char*)glyphBuffer, glyphBufferSize);
	}

	PSF1_FONT* finishedFont = new PSF1_FONT;
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;

	fs.close();

	std::cout << (int)finishedFont->psf1_Header->charsize << std::endl;
}
