// Not even finished :)
// Is really a C program

#include <stdlib.h>     // srand, rand
#include <time.h>		// time
#include <stdio.h>		// printing, NULL

const int IdleDelay = 250;

const int ArrayLength = 8;
const int ArrayBreak = ArrayLength / 2;
const int StartIndex = ArrayLength - 1;

int main(int argc, char* args[])
{
	srand(time(NULL)); //Set 'random' seed from time

	short word[2][ArrayLength] = { 0 };

	for (short i = 0; i < ArrayLength; i++)
	{
		word[0][i] = rand() % 16;

		if (i == ArrayBreak)
		{
			printf(" ");
		}

		printf("%01X", word[0][i]);
	}
}
