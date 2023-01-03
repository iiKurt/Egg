#pragma once

const int GameArrayHeight = 2;
const int GameArrayLength = 8;
const int StartIndex = GameArrayLength - 1;
#if defined(__IPHONEOS__) || defined(__ANDROID__)
const char* const TutorialText = "Tap anywhere to confirm current character...";
#else
const char* const TutorialText = "Press any key to confirm current character...\xA0";
#endif

struct Game_State {
	int Array[GameArrayHeight][GameArrayLength] = {{ 0 }};
	int CurrentIndex = StartIndex;
	bool Tutorial = true;
	bool Complete = false;
	int Acknowledgements = 0;
};

void Game_Initalise();
Game_State* Game_GetState();
bool Game_Submit();
void Game_Update();
const char* Game_GetEggText();
char Game_GetHexCharacter(int n);
int Game_CalculateTime();
