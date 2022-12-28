// TODOs:
// Consistent {} spacing and general formatting
// Huge project: seperate things into functions/classes? that have draw methods and properties such as color etc...
// Gamestates and all...
// File organisation and naming

// Using SDL and standard IO
#include <string>
#include <cstdlib> // srand, rand
#include <ctime> // time
#include <map>

#include <SDL.h>
#include "Font/LoadPSF.hpp"
#include "LoadTexture.hpp"
#include "DrawPSF.hpp"

#include "Resource.hpp"
#include "Resources.h"

// Game constants
const int ArrayHeight = 2;
const int ArrayLength = 8;
const int ArrayBreak = ArrayLength / 2;
const int StartIndex = ArrayLength - 1;

// Screen dimension constants (640 * 480)
int SCREEN_WIDTH = 410;
int SCREEN_HEIGHT = 240;

// Some global stuffs
int rendering = 0; // Look into "semaphores lazy foo SDL"
// The window we'll be rendering to
SDL_Window* window = NULL;	
// The surface contained by the window
SDL_Renderer* renderer = NULL;
// Font
PSF1_FONT* font = NULL;

// Textures
SDL_Texture* happyMac = NULL;
SDL_Texture* sadMac = NULL;
// Rectangles for drawing which will specify source (inside the texture) and target (on the screen) for rendering our textures.
// Dimensions of Mac image are 26 * 29
SDL_Rect SrcR = { 0, 0, 26, 29 };
SDL_Rect DestR = { 0, 0, 26, 29 };

// Function declarations
inline long map(long x, long in_min, long in_max, long out_min, long out_max);
inline unsigned int aSensibleTime();
Uint32 tickerCallback(Uint32 interval, void* param);
Uint32 wonCallback(Uint32 interval, void* param);
void update();
void render();

// Game things
int acknowledgedWin = 0;
unsigned int acknowledgedWinCount = 0;
int knowsHowToPlay = 0;
int won = 0;
SDL_TimerID ticker;
short currentIndex = StartIndex;
short word[ArrayHeight][ArrayLength] = {{ 0 }}; // Clang go brrrr

std::map<int, std::string> eggEggTexts = {
	{ 3, "You won!" },
	{ 4, "Yep..."},
	{ 5, "You sure did."},
	{ 6, "Would you like a trophy?"},
	{ 7, "Sorry, we're all out of stock."},
	{ 9, "Could you please stop pressing [Space]?"},
	{ 10, "ZZZzzzzZZZzzZZZzzZzz"},
	{ 11, "zzzZZZZzzzZZzzzZZzZZ"},
	{ 13, "Agh! I'm trying to sleep here..." },
	{ 15, "Go away please!" },
	{ 20, "I don't have anything of value here..." },
	{ 30, "I am impressed by your persistence." },
	{ 50, "Ok this is it, or is it?" },
	{ 100, "Do you really have nothing better to do?" },
	{ 500, "This must be getting old..." },
	{ 1000, "R.I.P. keyboard." },
	{ 2002, "Ayy" },
	{ 5000, "I don't have anything for you."},
	{ 10000, "Fine: 🏆"}, // 🏆
	{ 10001, "Yeah this isn't a unicode font... ain't no trophies here."},
	{ 10002, "Only corruption."},
	{ 10004, "~\\_(^_^)_/~"}
};

// Executes on a different thread
// https://stackoverflow.com/a/40693139
static int resizeEventWatcher(void* data, SDL_Event* event) {
	// Unused
	(void)data;

	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_RESIZED) {
		SCREEN_WIDTH = event->window.data1;
		SCREEN_HEIGHT = event->window.data2;
		render();
	}
	return 0;
}

int (int argc, char* argv[]) // int argc, char* args[]
{
	// Unused
	(void)argc;
	(void)argv;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		// Create window
		window = SDL_CreateWindow("Egg", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN|
			SDL_WINDOW_RESIZABLE);
		if (window == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else // Window creation successful
		{
			// Define window behaviour
			SDL_SetWindowMinimumSize(window, 410, 240); // 100, 70
			// Add some event watchers
			SDL_AddEventWatch(resizeEventWatcher, window);

			// Create a hardware accelerated renderer
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

			// Could have all the file resource loading stuff under one try block
			try {
				// Load font
				#ifdef _WIN32
				Resource fontR = Resource(RESOURCE_FONT_PATH, RESOURCE_FONT, FILE_PSF);
				#else
				Resource fontR = Resource(RESOURCE_FONT_PATH);
				#endif

				font = LoadPSF1Font(&fontR);

				// Load textures
				#ifdef _WIN32
				Resource happyMacR = Resource(RESOURCE_HAPPYMAC_PATH, RESOURCE_HAPPYMAC, FILE_BITMAP);
				Resource sadMacR = Resource(RESOURCE_SADMAC_PATH, RESOURCE_HAPPYMAC, FILE_BITMAP);
				#else
				Resource happyMacR = Resource(RESOURCE_HAPPYMAC_PATH);
				Resource sadMacR = Resource(RESOURCE_SADMAC_PATH);
				#endif

				happyMac = LoadTexture(renderer, &happyMacR);
				sadMac = LoadTexture(renderer, &sadMacR);
			}
			catch (const FileError& ex) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File load error: %s Filename: %s", ex.what(), ex.path);
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"File load error",
					(std::string() + ex.what() + "\r\nFilename: " + ex.path).c_str(),
					window);
				return 1;
			}

			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Successfully loaded"); // Shorthand: SDL_Log("whatever");
			
			// Set color
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			// Fill screen with color
			SDL_RenderClear(renderer);
			
			// So we don't have to wait for the timer to finish to see the render
			render();

			///////////////////////////////////////////////////////////////////////////////////////////////////

			srand((unsigned)time(0));

			// Populate top row of numbers
			for (short i = 0; i < ArrayLength; i++) {
				word[0][i] = rand() % 16;
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////

			SDL_Event event;
			ticker = SDL_AddTimer(aSensibleTime(), tickerCallback, NULL);
			int running = 1;
			while (running)
			{
				if (SDL_WaitEvent(&event) != 0) {
					switch (event.type) {
						case SDL_KEYDOWN:
							switch (event.key.keysym.sym) {
								case SDLK_SPACE:
									if (won) {
										acknowledgedWinCount++;
									}
									else { // Don't process key stuff
										knowsHowToPlay = 1;
										if (word[1][currentIndex] == word[0][currentIndex]) {
											if (--currentIndex < 0) { // End of the row, game complete
												won = 1; // heh
												SDL_RemoveTimer(ticker);
												ticker = SDL_AddTimer(500, wonCallback, NULL);
												render(); // show that happy mac!
												break;
											}
										}
										else if (currentIndex < StartIndex) { // Pressed at wrong time
											currentIndex++; // Go back
										}
										update();
									}
									render();
								break;
							}
							break;
						// Handle OS level events
						case SDL_QUIT:
							running = 0;
							break;
					}
				}
			}
		}
	}

	// Remove timer
	SDL_RemoveTimer(ticker);

	// Destory renderer
	SDL_DestroyRenderer(renderer);

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}

// Utility functions
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline unsigned int aSensibleTime() {
	//return map(currentIndex, 0, StartIndex, 500, 1500); // Hard mode
	return map(currentIndex, 0, StartIndex, 1500, 500); // EZ mode
}

Uint32 tickerCallback(Uint32 interval, void* param) {
	// Unused
	(void)interval;
	(void)param;

	SDL_RemoveTimer(ticker);
	ticker = SDL_AddTimer(aSensibleTime(), tickerCallback, NULL);
	update();
	render();
	return 0;
}

Uint32 wonCallback(Uint32 interval, void* param) {
	// Unused
	(void)interval;
	(void)param;

	SDL_RemoveTimer(ticker);
	acknowledgedWin = 1;
	render();
	return 0;
}

// Update and render methods
void update() {
	word[1][currentIndex] = rand() % 16; // Generate random number
}

void render() {
	if (rendering) {
		return;
	}
	rendering = 1;

	if (!knowsHowToPlay) {
		SDL_SetRenderDrawColor(renderer, 0, 192, 224, 255);
		// 45 characters in help text, each one is 8 px long:
		PrintString(renderer, font, "Press [Space] to confirm current character...", (SCREEN_WIDTH / 2) - ((45 * 8) / 2), 20);
	}

	// Draw Mac (26 * 29 image)
	DestR.x = (SCREEN_WIDTH / 2) - (26 / 2);
	DestR.y = (SCREEN_HEIGHT / 2) - ((29 / 2) * 3);

	if (won) {
		SDL_RenderCopy(renderer, happyMac, &SrcR, &DestR);

		if (eggEggTexts.count(acknowledgedWinCount) == 1) { // Item exists
			// Do the eggEgg stuffs
			SDL_SetRenderDrawColor(renderer, 0, 192, 224, 255);
			std::string text = eggEggTexts[acknowledgedWinCount];

			// Calculate how many px the string would be if each char is 8 px long
			PrintString(renderer, font, text.c_str(), (SCREEN_WIDTH / 2) - ((text.length() * 8) / 2), 20);
			//std::cout << eggEggTexts[acknowledgedWinCount] << std::endl;
		}
	}
	else {
		SDL_RenderCopy(renderer, sadMac, &SrcR, &DestR);
	}

	if (won && !acknowledgedWin) {
		// Only applies to text
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	for (short i = 0; i < ArrayLength; i++) { // For each column
		int x; // https://stackoverflow.com/a/8735780
		int y;

		for (int j = 0; j < ArrayHeight; j++) // For each row
		{
			// First number = offset
			// Second number - 6 = padding between each char
			// 10, 10
			x = ((SCREEN_WIDTH / 2) - ((ArrayLength / 2) * 10)) + (10 * i);
			// 14, 16
			y = ((SCREEN_HEIGHT / 2) - ((ArrayHeight / 2) * -6)) + (16 * j);

			if (i < ArrayBreak) // Left side
			{
				x -= 2;
			}
			else {
				x += 2;
			}

			PrintCharacter(renderer, font, (word[j][i] > 9) ? (word[j][i] - 10) + 'A' : word[j][i] + '0', x, y);
		}

		if (currentIndex == i) {
			SDL_RenderDrawLine(renderer, x, y + 14, x + 7, y + 14);
		}
	}

	// Draw any pending data to the screen
	SDL_RenderPresent(renderer);

	// Clear the screen for next time
	// The backbuffer should be considered invalidated after each present;
	// do not assume that previous contents will exist between frames.

	// Set color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// Fill screen with color
	SDL_RenderClear(renderer);

	rendering = 0;
}
