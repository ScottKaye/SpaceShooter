#pragma once

#include "Animation.h"
#include "Vec2.h"

class Explosion {
	Animation*              mAnim;
	Vec2                    mCenter;
	float					mScale;

public:
	Explosion(const Vec2& pos, const Texture* tex, float scale);
	~Explosion();

	void					Update(float dt);
	bool                    IsFinished() const;
	void					Draw(SDL_Renderer* renderer) const;
};