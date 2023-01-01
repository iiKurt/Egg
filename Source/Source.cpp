#include <SDL.h>
#include <string>

#include "Resources.h"
#include "LoadTexture.hpp"
#include "BMPF.hpp"

// Screen dimension constants (640 * 480)
int SCREEN_WIDTH = 410;
int SCREEN_HEIGHT = 240;

// The window we'll be rendering to
SDL_Window* window = NULL;	
// The surface contained by the window
SDL_Renderer* renderer = NULL;

void quit();

int main(int argc, char* argv[]) // int argc, char* args[]
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
        #if defined(__IPHONEOS__) || defined(__ANDROID__)
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        
        SCREEN_HEIGHT = dm.h;
        SCREEN_WIDTH = dm.w;
        #endif
        
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
			// Add some event watchers
			//SDL_AddEventWatch(resizeEventWatcher, window);

			// Create a hardware accelerated renderer
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
			
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
					quit();
					return -1;
				}
//			}
            
            // https://discourse.libsdl.org/t/high-dpi-mode/34411/2
            int rendererWidth = 0, rendererHeight = 0;
            SDL_GetRendererOutputSize(renderer, &rendererWidth, &rendererHeight);
			
            if (rendererWidth != SCREEN_WIDTH) {
                float widthScale = (float)rendererWidth / (float) SCREEN_WIDTH;
                float heightScale = (float)rendererHeight / (float) SCREEN_HEIGHT;

                if (widthScale != heightScale) {
                    fprintf(stderr, "WARNING: width scale != height scale\n");
                }
                
                //#if defined(__IPHONEOS__) || defined(__ANDROID__)
                //SDL_RenderSetScale(renderer, widthScale * 2, heightScale * 2);
                //SCREEN_HEIGHT /= 2;
                //SCREEN_WIDTH /= 2;
                //#else
                SDL_RenderSetScale(renderer, widthScale, heightScale);
                //#endif
            }

			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Successfully loaded"); // Shorthand: SDL_Log("whatever");
		}
	}
	
	// Set up font
	SDL_Texture* bmpTexture = LoadTexture(renderer, RESOURCE_FONT);
	if (bmpTexture == NULL)
	{
		// Unable to load as a BMP
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File load error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"File load error",
			"Unable to load file.",
			window);
		quit();
		return -1;
	}
	BMPF_initalise(bmpTexture); // Use bitmap font
	
	SDL_Event event;
	bool running = true;
	while (running && SDL_WaitEvent(&event))
	{
		// Clear the screen
		// The backbuffer should be considered invalidated after each present;
		// do not assume that previous contents will exist between frames.

		// Set background color to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// Fill screen with color
		SDL_RenderClear(renderer);
		
		// Set foreground color to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		BMPF_print(renderer, "Testing!", 100, 200, false);
		
		if (event.type == SDL_QUIT) {
			running = false;
			break;
		}
		
		// Draw any pending data to the screen
		SDL_RenderPresent(renderer);
	}

	return 0;
}

void quit() {
	// Destory renderer
	SDL_DestroyRenderer(renderer);

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();
}
