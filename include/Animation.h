#pragma once

#include <SDL.h>

#include "Texture.h"

class Animation {
	const Texture*              mTex;

	int                         mNumCells;
	int                         mCellWidth;
	int                         mCellHeight;

	float                       mDuration;          // Total duration in seconds
	float                       mTime;              // Current time position (in seconds)

	bool                        mIsLoopable;

public:
	Animation(const Texture* tex, int numCells, float duration, bool loopable);

	int                         CellWidth() const { return mCellWidth; }
	int                         CellHeight() const { return mCellHeight; }

	void                        AddTime(float dt);

	bool                        FinishedPlaying() const;

	void                        DrawCurrentCell(SDL_Renderer* renderer, const SDL_Rect* dstRect);
};