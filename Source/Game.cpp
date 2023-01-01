#include "Game.hpp"

#include <ctime>

static bool TutorialTextActive = false;
static bool GameComplete = false;
static int AcknowledgementCount = 0;

int map(int x, int in_min, int in_max, int out_min, int out_max);

void Game_Initalise() {
	srand((unsigned)time(0));

	// Populate top row of numbers
	for (short i = 0; i < GameArrayLength; i++) {
		GameArray[0][i] = rand() % 16;
	}
}

void Game_Submit() {
	TutorialTextActive = false;

	if (GameComplete) {
		AcknowledgementCount++;
		return;
	}

	if (GameArray[1][GameCurrentIndex] == GameArray[0][GameCurrentIndex]) {
		if (GameCurrentIndex - 1 < 0) { // End of the row, game complete
			GameComplete = true;
		}
		else {
			GameCurrentIndex--;
		}
	}
	else if (GameCurrentIndex < StartIndex) { // Pressed at wrong time
		GameCurrentIndex++; // Go back
	}
}

// Utility functions
char Game_GetHexCharacter(int n) {
	return (n > 9) ? (n - 10) + 'A' : n + '0';
}

int CalculateDelay() {
	//return map(CurrentIndex, 0, StartIndex, 500, 1500); // Hard mode
	return map(GameCurrentIndex, 0, StartIndex, 1500, 500); // EZ mode
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
