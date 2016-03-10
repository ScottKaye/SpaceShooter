#pragma once

#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace System {
	SDL_Window*     GetWindow();
	int             GetWindowWidth();
	int             GetWindowHeight();

	void            SetWindowSize(int w, int h);
	void            SetWindowTitle(const char* title);

	SDL_Renderer*   GetRenderer();
	const Uint8*    GetKeyStates();

	float           GetTime();
	float           GetTimeSinceLastFrame();

	Uint64          GetFrameNumber();

	void            Quit();

	void            Pause();
	void            Unpause();

	bool            IsPaused();
}