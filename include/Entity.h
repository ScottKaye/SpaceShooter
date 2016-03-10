#pragma once

#include <vector>
#include <math.h>

#include "Team.h"
#include "Vec2.h"
#include "Texture.h"

class Entity {
	const Texture*			mTex;
	float					mHealth;
	SDL_Rect				mHealthRect;
	Team					mTeam;
	unsigned				mId;
	static unsigned			mNextId;
	float					mExplosionSize;
	bool					mHealthBarVisible;
	int						mLifetime;

public:
	Entity(const Vec2& pos, const Texture* tex, Team team);

	unsigned				GetId() const { return mId; }
	Team					GetTeam() const { return mTeam; }
	float					Health() const { return mHealth; }
	SDL_Rect*				HealthRect() { return &mHealthRect; }
	bool					Hit(float damage);
	void					Heal(float amount);
	void					Explode();

	Vec2					Center;
	Vec2					Velocity;
	bool					Alive = true;				// If an entity is not alive, it will not be drawn and will not collide with other entities
	bool					KillOnOffscreen = true;		// If an entity goes offscreen, kill it?
	bool					Garbage = false;			// Used to define if an entity should be deleted.  Entities are kept around until the beginning of the next frame
	float					CollisionDamage = 100;		// How much damage to deal to the entity that collides with this

	float                   Left() const { return Center.x - 0.5f * Width(); }
	float                   Right() const { return Center.x + 0.5f * Width(); }
	float                   Top() const { return Center.y - 0.5f * Height(); }
	float                   Bottom() const { return Center.y + 0.5f * Height(); }
	bool					OffScreen() const;
	void					ConfineToScreen();

	int						Lifetime() const { return mLifetime; }

	void					SetExplosionSize(float s) { mExplosionSize = s; }
	void					SetHealth(float h) { mHealth = h; }
	void					SetHealthBarVisibile(bool visible) { mHealthBarVisible = visible; }
	void                    SetLeft(float x) { Center.x = x + 0.5f * Width(); }
	void                    SetRight(float x) { Center.x = x - 0.5f * Width(); }
	void                    SetTop(float y) { Center.y = y + 0.5f * Height(); }
	void                    SetBottom(float y) { Center.y = y - 0.5f * Height(); }

	float					Radius() const { return fmaxf(Width(), Height()) / 2; }
	float                   Width() const { return (float)mTex->GetWidth(); }
	float                   Height() const { return (float)mTex->GetHeight(); }

	// Implementation-specific methods
	virtual bool			Update(float dt);
	virtual void			Destroy();
	virtual void			Draw(SDL_Renderer* renderer) const;
	virtual bool			CollidesWith(Entity* ent) const;
	virtual void			Shoot();
};