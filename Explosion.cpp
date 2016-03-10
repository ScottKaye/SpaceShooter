#include <iostream>

#include "Explosion.h"
#include "System.h"

Explosion::Explosion(const Vec2& pos, const Texture* tex, float scale)
    : mAnim(NULL)
    , mCenter(pos)
	, mScale(scale)
{
    if (tex) {
        mAnim = new Animation(tex, 16, 0.85f, false);
    }
}

Explosion::~Explosion()
{
    //std::cout << "Destroying Explosion" << std::endl;

    delete mAnim;   // ok even if NULL
}

void Explosion::Update(float dt)
{
    if (mAnim) {
        mAnim->AddTime(dt);
    }
}

bool Explosion::IsFinished() const
{
	return mAnim ? mAnim->FinishedPlaying() : true;
}

void Explosion::Draw(SDL_Renderer* renderer) const
{
    if (mAnim) {
        SDL_Rect screenRect;
        screenRect.w = mAnim->CellWidth();
        screenRect.h = mAnim->CellHeight();
        screenRect.x = (int)(mCenter.x - mAnim->CellWidth() / (2 / mScale));
        screenRect.y = (int)(mCenter.y - mAnim->CellHeight() / (2 / mScale));

		screenRect.w = (int)(screenRect.w * mScale);
		screenRect.h = (int)(screenRect.h * mScale);

        mAnim->DrawCurrentCell(renderer, &screenRect);
    } else {
        // draw a placeholder
        SDL_Rect screenRect;
        screenRect.w = 64;
        screenRect.h = 64;
        screenRect.x = (int)(mCenter.x - screenRect.w / 2);
        screenRect.y = (int)(mCenter.y - screenRect.h / 2);

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &screenRect);
    }
}
