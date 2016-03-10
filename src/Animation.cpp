// Animation class created by Lukasz Wawrzyniak

#include <cmath>

#include "Animation.h"

Animation::Animation(const Texture* tex, int numCells, float duration, bool loopable)
	: mTex(tex)
	, mNumCells(numCells)
	, mCellWidth(tex->GetWidth() / numCells)
	, mCellHeight(tex->GetHeight())
	, mDuration(duration)
	, mTime(0.0f)
	, mIsLoopable(loopable)
{
}

void Animation::AddTime(float dt) {
	mTime += dt;

	// See if we've reached or passed the end
	if (mTime >= mDuration) {
		if (mIsLoopable) {
			// Wrap around
			mTime = std::fmod(mTime, mDuration);
		}
		else {
			// Cap
			mTime = mDuration;
		}
	}
}

bool Animation::FinishedPlaying() const {
	return mTime == mDuration;
}

void Animation::DrawCurrentCell(SDL_Renderer* renderer, const SDL_Rect* dstRect) {
	// Figure out which cell we should be showing
	int cellIndex;
	if (mTime <= 0.0f) {
		cellIndex = 0;
	}
	else if (mTime >= mDuration) {
		cellIndex = mNumCells - 1;
	}
	else {
		cellIndex = (int)(mTime / mDuration * mNumCells);
	}

	// Compute source rectangle of cell in the texture
	SDL_Rect srcRect;
	srcRect.x = cellIndex * mCellWidth;
	srcRect.y = 0;
	srcRect.w = mCellWidth;
	srcRect.h = mCellHeight;

	// Draw current cell in the specified screen rectangle
	SDL_RenderCopy(renderer, mTex->GetSDLTexture(), &srcRect, dstRect);
}