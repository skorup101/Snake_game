#include "Game.h"
#include <windows.h>

void getRidOfConsole();

int main()
{
	getRidOfConsole();

	Game game;
	game.run();
}

void getRidOfConsole() {
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
	FreeConsole();
}
