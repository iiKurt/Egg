#include <SDL.h>

#include "Resources.h"
#include "LoadTexture.hpp"
#include "BMPF.hpp"
#include "Game.hpp"

// Screen dimension constants (640 * 480)
int SCREEN_WIDTH = 420;
int SCREEN_HEIGHT = 240;

// The window we'll be rendering to
SDL_Window* window = NULL;	
// The surface contained by the window
SDL_Renderer* renderer = NULL;

// Resources
SDL_Texture* happyMac;
SDL_Texture* sadMac;

// Game state
Game_State* gameState;
SDL_TimerID timer;

// Function prototypes
void setScreenSize();
bool initalise();
Uint32 timerCallback(Uint32 interval, void* param);
void update();
void render();
void quit();

void setScreenSize() {
	// Related: https://discourse.libsdl.org/t/high-dpi-mode/34411/2
	// Display scaling for high dpi displays
	
	// Scaled window size (a high dpi will report this as a smaller number than its screen)
	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool initalise() {
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		// Create window
		window = SDL_CreateWindow("Egg", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
		                          SDL_WINDOW_SHOWN |
		                          SDL_WINDOW_RESIZABLE |
		                          SDL_WINDOW_ALLOW_HIGHDPI);
		
		if (window == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else // Window creation successful
		{
			// Define window behaviour
			SDL_SetWindowMinimumSize(window, 410, 240); // 100, 70
			
			// Create a hardware accelerated renderer
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
			
			setScreenSize();
			
//			if (renderer == NULL) {
//				// Error creating renderer, try a software one instead
//				renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
//				// SDL_UpdateWindowSurface(window); is needed to draw, rather than SDL_RenderPresent();
			
			// Still an error? Give up
			if (renderer == NULL) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created. SDL Error: %s", SDL_GetError());
				SDL_ShowSimpleMessageBox(
				                         SDL_MESSAGEBOX_ERROR,
				                         "Renderer error",
				                         "Renderer could not be created.",
				                         window);
				return false;
			}
//			}
		}
	}
	
	// Set up font
	SDL_Texture* bmpTexture = LoadTexture(renderer, RESOURCE_FONT);
	happyMac = LoadTexture(renderer, RESOURCE_HAPPYMAC);
	sadMac = LoadTexture(renderer, RESOURCE_SADMAC);
	if (bmpTexture == NULL ||
		happyMac == NULL ||
		sadMac == NULL)
	{
		// Unable to load as a BMP
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Resource load error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(
		                         SDL_MESSAGEBOX_ERROR,
		                         "Resource load error",
		                         "Unable to load Resource.",
		                         window);
		return false;
	}
	BMPF_Initalise(bmpTexture); // Use bitmap font
	
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Successfully loaded"); // Shorthand: SDL_Log("whatever");
	
	Game_Initalise();
	gameState = Game_GetState();
	
	timer = SDL_AddTimer(Game_CalculateTime(), timerCallback, NULL);
	
	return true;
}
	
int main(int argc, char* argv[])
{
	// Unused
	(void)argc;
	(void)argv;

	// Initalisation failed
	if (!initalise()) {
		// Quit program
		quit();
		return -1;
	}
	
	// Render now so we don't have to wait for any events
	render();
	
	SDL_Event event;
	bool running = true;
	while (running && SDL_WaitEvent(&event))
	{
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_FINGERDOWN:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_KEYDOWN:
				if (event.key.repeat != 0) {
					break;
				}
				
				if (Game_Submit()) {
					SDL_RemoveTimer(timer);
					update();
					render();
					timer = SDL_AddTimer(Game_CalculateTime(), timerCallback, NULL);
				}
				else {
					render();
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) {
					break;
				}
			case SDL_DISPLAYEVENT_ORIENTATION:
				// Update screen size variables when window size has changed or display has been rotated
				setScreenSize();
				render();
				break;
				
			default:
				break;
		}
	}

	return 0;
}

Uint32 timerCallback(Uint32 interval, void* param) {
	// Unused
	(void)interval;
	(void)param;
	
	SDL_RemoveTimer(timer);
	timer = SDL_AddTimer(Game_CalculateTime(), timerCallback, NULL);
	
	update();
	render();
	return 0;
}

void update() {
	// Stop updating the game and its random numbers if complete
	if (gameState->Complete) {
		SDL_RemoveTimer(timer);
	}
	else {
		Game_Update();
	}
}

void render() {
	// Clear the screen
	// The backbuffer should be considered invalidated after each present;
	// do not assume that previous contents will exist between frames.
	
	// Set background color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// Fill screen with color
	SDL_RenderClear(renderer);
	
	// Turorial text
	if (gameState->Tutorial) {
		BMPF_SetColor(0, 192, 224);
		BMPF_Print(renderer, TutorialText, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 100, true);
	}
	
	// Draw Mac (26 * 29 image)
	SDL_Rect SrcR = { 0, 0, 26, 29 };
	SDL_Rect DestR = { 0, 0, 26, 29 };
	
	DestR.x = (SCREEN_WIDTH / 2) - (26 / 2);
	DestR.y = (SCREEN_HEIGHT / 2) - ((29 / 2) * 3);
	
	// Happy Mac
	if (gameState->Complete) {
		SDL_RenderCopy(renderer, happyMac, &SrcR, &DestR);
	}
	// Sad Mac
	else {
		SDL_RenderCopy(renderer, sadMac, &SrcR, &DestR);
	}
	
	// Set foreground color to white
	BMPF_SetColor(255, 255, 255);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Draw error codes
	for (short i = 0; i < GameArrayLength; i++) { // For each column
		int x; // https://stackoverflow.com/a/8735780
		int y;

		for (int j = 0; j < GameArrayHeight; j++) // For each row
		{
			// First number = offset
			// Second number - 6 = padding between each char
			// 10, 10
			x = ((SCREEN_WIDTH / 2) - ((GameArrayLength / 2) * 10)) + (10 * i);
			// 14, 16
			y = ((SCREEN_HEIGHT / 2) - ((GameArrayHeight / 2) * -6)) + (16 * j);

			if (i < GameArrayLength / 2) // Left side
			{
				x -= 2;
			}
			else {
				x += 2;
			}

			BMPF_Print(renderer, Game_GetHexCharacter(gameState->Array[j][i]), x, y);
			
			// Underline current character, only on the second row
			if (gameState->CurrentIndex == i && j == 1 && !gameState->Complete) {
				SDL_RenderDrawLine(renderer, x, y + 14, x + 7, y + 14);
			}
		}
	}
	
	// Easter egg stuff
	if (gameState->Complete) {
		// Easter egg text
		BMPF_SetColor(0, 192, 224);
		BMPF_Print(renderer, Game_GetEggText(), (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 100, true);
		
		// Counter
	}
	
	// Draw any pending data to the screen
	SDL_RenderPresent(renderer);
}

void quit() {
	BMPF_Kill();
	
	// Destory renderer
	SDL_DestroyRenderer(renderer);

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();
}
