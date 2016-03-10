#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Team.h"
#include "Vec2.h"
#include "Random.h"
#include "Util.h"

class Player : public Entity {
	float                   mSpeed;
	float					mSpread;
	int						mCannons;
	float					mEnergy;
	float					mMissileSpeed;

public:
	Player(const Vec2& pos, const Texture* tex);

	bool					Alive;

	float					Energy() const { return mEnergy; }
	float					Spread() const { return mSpread; }
	float					MissileSpeed() const { return mMissileSpeed; }
	int						Cannons() const { return mCannons; }
	bool					CannonJammed() const { return RandomFloatInclusive(0, 100) > mEnergy; }
	void					DrainEnergy(float amount);
	void					RestoreEnergy(float amount);

	void					AddCannon() { clamp(++mCannons, 1, 20); }
	void					RemoveCannon() { clamp(--mCannons, 1, 20); }
	void					SetCannons(int cannons) { mCannons = cannons; }

	float                   Speed() const { return mSpeed; }
	void                    SetSpeed(float speed) { mSpeed = speed; }

	virtual bool			CollidesWith(Entity* ent) const;
	virtual bool			Update(float dt);
	virtual void			Draw(SDL_Renderer* renderer) const;
	virtual void			Destroy();
	virtual void			Shoot();
};