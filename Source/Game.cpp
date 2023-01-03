#include "Game.hpp"

#include <map>     // Map
#include <ctime>   // Time
#include <cstdlib> // Random

Game_State state;
std::map<int, const char*> eggEggTexts = {
	{ 0, "" },
	{ 3, "You won!" },
	{ 4, "Yep..."},
	{ 5, "You sure did."},
	{ 6, "Would you like a trophy?"},
	{ 7, "Sorry, we're all out of stock."},
#if defined(__IPHONEOS__) || defined(__ANDROID__)
	{ 9, "Could you please stop pressing keys?"},
#else
	{ 9, "Could you please stop tapping?"},
#endif
	{ 10, "ZZZzzzzZZZzzZZZzzZzz"},
	{ 11, "zzzZZZZzzzZZzzzZZzZZ"},
	{ 13, "Agh! I'm trying to sleep here..." },
	{ 15, "Go away please!" },
	{ 20, "I don't have anything of value here..." },
	{ 30, "I am impressed by your persistence." },
	{ 50, "Ok this is it, or is it?" },
	{ 100, "Do you really have nothing better to do?" },
	{ 500, "This must be getting old..." },
#if defined(__IPHONEOS__) || defined(__ANDROID__)
	{ 1000, "R.I.P. keyboard." },
#else
	{ 1000, "R.I.P. screen." },
#endif
	{ 2002, "Ayy" },
	{ 5000, "I don't have anything for you."},
	{ 10000, "Fine: \xA0"}, // 🏆
	{ 10001, "Yeah this isn't a unicode font... ain't no trophies here."},
	{ 10002, "Only corruption."},
	{ 10004, "~\\_(^_^)_/~"}
};
int lastAcknowledgement = 0;

int map(int x, int in_min, int in_max, int out_min, int out_max);

void Game_Initalise() {
	srand((unsigned)time(0));
	
	// Populate top row of numbers
	for (short i = 0; i < GameArrayLength; i++) {
		state.Array[0][i] = rand() % 16;
	}
}

Game_State* Game_GetState() {
	return &state;
}

bool Game_Submit() {
	state.Tutorial = false;

	// Game already complete
	if (state.Complete) {
		state.Acknowledgements++;
		return false;
	}

	if (state.Array[1][state.CurrentIndex] == state.Array[0][state.CurrentIndex]) {
		if (state.CurrentIndex - 1 < 0) { // End of the row, game complete
			state.Complete = true;
			return false;
		}
		else { // Correct, move forward
			state.CurrentIndex--;
			return false;
		}
	}
	else { // Pressed at wrong time
		// Only go back if not already at the starting point
		if (state.CurrentIndex < StartIndex) {
			state.CurrentIndex++; // Go back
			return true;
		}
		return false;
	}
}

void Game_Update() {
	// 1 in 5 chance (technically slightly more) to have the correct answer inserted
	if (rand() % 5 == 0) {
		state.Array[1][state.CurrentIndex] = state.Array[0][state.CurrentIndex];
	}
	else {
		state.Array[1][state.CurrentIndex] = rand() % 16;
	}
}

const char* Game_GetEggText() {
	if (eggEggTexts.count(state.Acknowledgements) == 1) { // Item exists
		lastAcknowledgement = state.Acknowledgements;
		return eggEggTexts[state.Acknowledgements];
	}
	else {
		return eggEggTexts[lastAcknowledgement];
	}
}

// Utility functions
char Game_GetHexCharacter(int n) {
	return (n > 9) ? (n - 10) + 'A' : n + '0';
}

int Game_CalculateTime() {
	//return map(CurrentIndex, 0, StartIndex, 500, 1500); // Hard mode
	return map(state.CurrentIndex, 0, StartIndex, 1500, 500); // EZ mode
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
