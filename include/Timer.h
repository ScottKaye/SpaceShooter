#pragma once

#include <SDL_timer.h>

class Timer {
	Uint32 mStartTime;
	Uint32 mPauseTime;
	bool mIsPaused;

public:
	Timer()
		: mStartTime(0)
		, mPauseTime(0)
		, mIsPaused(true)
	{ }

	void Start() {
		mStartTime = SDL_GetTicks();
		mIsPaused = false;
	}

	void Pause() {
		mPauseTime = SDL_GetTicks();
		mIsPaused = true;
	}

	void Unpause() {
		Uint32 now = SDL_GetTicks();
		Uint32 timeSincePause = now - mPauseTime;

		// Shift start time to account for time spent while paused
		mStartTime += timeSincePause;
	}

	float GetTime() const {
		if (!mIsPaused) {
			Uint32 now = SDL_GetTicks();
			return 0.001f * (now - mStartTime);
		}
	
		return 0.001f * (mPauseTime - mStartTime);
	}
};