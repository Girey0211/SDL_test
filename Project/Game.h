#pragma once
#include <SDL.h>

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
	Vector2 mPaddlePos;
	Vector2 mBallVel;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateInput();

	SDL_Window* mWindow;
	bool mIsRunning;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;
	int mPaddleDir;
	

};