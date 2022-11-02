#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <ctime>

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();

	bool Initialize();
	void Loop();
	void shutdown();

	Vector2 mBallPos;
	Vector2 mPaddlePosL;
	Vector2 mPaddlePosR;
	Vector2 mBallVel;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateInput();

	SDL_Window* mWindow;
	bool mIsRunning;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;
	int mPaddleDirL;
	int mPaddleDirR;
	
};