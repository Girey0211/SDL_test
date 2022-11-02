#include "Game.h"

#define height      768.f
#define width       1024.f
#define thickness   15
#define paddleH     100.f
#define paddleSpeed 300.f

Game::Game(){
	mWindow     = nullptr;
	mIsRunning  = true;
	mRenderer   = nullptr;
	mBallPos    = {      width / 2 + 200.f, static_cast<float>(rand() % 300 + 234) };
	mPaddlePosL = {                     10, height/2 };
	mPaddlePosR = { width - 10 - thickness, height/2 };
	mTicksCount = 0;
	mBallVel    = { -350.f, 350.f };
}

bool Game::Initialize(){
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL : %s\n", SDL_GetError);
		return false;
	}
	mWindow = SDL_CreateWindow(
		"ping pong game",
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

	// update left paddle direction
	mPaddleDirL = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDirL -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDirL += 1;
	}
	// update right paddle direction
	mPaddleDirR = 0;
	if (state[SDL_SCANCODE_UP]) {
		mPaddleDirR -= 1;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		mPaddleDirR += 1;
	}
}
void Game::UpdateGame(){
	// set frame per second 30
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// set delta time
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.5f) {
		deltaTime = 0.5f;
	}

	// Update ticj counts
	mTicksCount = SDL_GetTicks();

	// update left paddle position
	if (mPaddleDirL != 0) {
		mPaddlePosL.y += mPaddleDirL * paddleSpeed * deltaTime;
		if (mPaddlePosL.y < (paddleH / 2.f + thickness)) {
			mPaddlePosL.y = paddleH / 2.f + thickness;
		}
		else if (mPaddlePosL.y > (height - paddleH / 2.f - thickness)) {
			mPaddlePosL.y = height - paddleH / 2.f - thickness;
		}
	}
	// update right paddle position
	if (mPaddleDirR != 0) {
		mPaddlePosR.y += mPaddleDirR * paddleSpeed * deltaTime;
		if (mPaddlePosR.y < (paddleH / 2.f + thickness)) {
			mPaddlePosR.y = paddleH / 2.f + thickness;
		}
		else if (mPaddlePosR.y > (height - paddleH / 2.f - thickness)) {
			mPaddlePosR.y = height - paddleH / 2.f - thickness;
		}
	}

	// update ball position
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	
	float diffL = mPaddlePosL.y - mBallPos.y;
	float diffR = mPaddlePosR.y - mBallPos.y;
	if (
		diffL       <= paddleH / 2.f &&
		mBallPos.x <= 25.f &&
		mBallPos.x >= 20.f &&
		mBallVel.x <  0.f
		) 
	{
		mBallVel.x *= -1.f;
		mBallVel.x += 3.f;
		if (mBallVel.y > 0) mBallVel.y += 3.f;
		else                mBallVel.y -= 3.f;
	} 
	// did the ball collide with the right paddle?
	else if (
		diffR <= paddleH / 2.f &&
		mBallPos.x <= width - thickness &&
		mBallPos.x >= width - 10 - thickness &&
		mBallVel.x > 0.f
		) 
	{
		mBallVel.x *= -1.f;
		mBallVel.x -= 3.f;
		if (mBallVel.y > 0) mBallVel.y += 3.f;
		else                mBallVel.y -= 3.f;
	}
	// did the ball go off the screen?
	else if (mBallPos.x <= 0.f || mBallPos.x >= width) {
		mIsRunning = false;
	} 

	// did the ball collide with the    top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.f)
		mBallVel.y *= -1;
	// did the ball collide with the bottom wall?
	else if (mBallPos.y >= (height - thickness) && mBallVel.y > 0.f)
		mBallVel.y *= -1;
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

	// draw left paddle
	SDL_Rect paddleL{
		static_cast<int>(mPaddlePosL.x),
		static_cast<int>(mPaddlePosL.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleL);
	// draw right paddle
	SDL_Rect paddleR{
		static_cast<int>(mPaddlePosR.x),
		static_cast<int>(mPaddlePosR.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleR);

	SDL_RenderPresent(mRenderer);
}
