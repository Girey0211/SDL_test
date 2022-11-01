#include "Game.h"

#define height    768.f
#define width     1024.f
#define thickness 15
#define paddleH   100.f
#define speed     300.f

Game::Game(){
	mWindow     = nullptr;
	mIsRunning  = true;
	mRenderer   = nullptr;
	mBallPos    = { width/2, height/2 };
	mPaddlePos  = { 30 , height/2 };
	mTicksCount = 0;
	mBallVel    = { -200.f, 235.f };
}

bool Game::Initialize(){
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL : %s\n", SDL_GetError);
		return false;
	}
	mWindow = SDL_CreateWindow(
		"game Programing in C++ (Chapter 1)",
		100,
		100,
		width,
		height,
		0
	);
	if (!mWindow) {
		SDL_Log("Failed to create window : %s\n", SDL_GetError);
		return false;
	}
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer : %s\n", SDL_GetError);
		return false;
	}
	return true;
}
void Game::Loop(){
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateInput();
	}
}
void Game::shutdown(){
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput(){
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}
}
void Game::UpdateGame(){
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.5f) {
		deltaTime = 0.5f;
	}
	mTicksCount = SDL_GetTicks();

	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * speed * deltaTime;
		if (mPaddlePos.y < (paddleH / 2.f + thickness))
			mPaddlePos.y = paddleH / 2.f + thickness;
		else if (mPaddlePos.y > (height - paddleH / 2.f - thickness))
			mPaddlePos.y = height - paddleH / 2.f - thickness;
	}

}
void Game::GenerateInput() {
	// background color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	SDL_RenderClear(mRenderer);

	//color change [black]
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	// draw top wall
	SDL_Rect wall{ 0, 0, 1024, thickness };
	SDL_RenderFillRect(mRenderer, &wall);
	// draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	// draw right wall
	wall = { 1024 - thickness, 0, thickness, 1024 };
	SDL_RenderFillRect(mRenderer, &wall);

	//color change [white]
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);


	// draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);
}
