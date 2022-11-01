#include <stdio.h>
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char* argv[]) {
	Game game;
	bool success = game.Initialize();
	if (success) {
		game.Loop();
	}
	game.shutdown();
	return 0;
}