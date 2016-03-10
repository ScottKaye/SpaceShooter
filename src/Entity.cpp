#include <iostream>

#include "Game.h"
#include "Entity.h"

// Resolve externs
unsigned Entity::mNextId;
Mix_Chunk* Game::ShotSound;

Entity::Entity(const Vec2& pos, const Texture* tex, Team team)
	: mTex(tex)
	, Center(pos)
	, mTeam(team)
	, mHealth(100.F)
	, mHealthRect({ 0, 0, 0, 3 })
	, mId(mNextId++)
	, mLifetime(0)
{
}

// Return true if the entity was destroyed this frame
bool Entity::Update(float dt) {
	// "Dead" entities are in limbo
	if (!Alive) return false;

	++mLifetime;

	Center.x += Velocity.x * dt;
	Center.y += Velocity.y * dt;

	if (mHealthBarVisible) {
		mHealthRect.x = (int)(Center.x - Width() / 2);
		mHealthRect.y = (int)Center.y;
		mHealthRect.w = (int)(mHealth / 100.F * Width());
	}

	// Confine to screen
	if (KillOnOffscreen && Lifetime() > (60 * 5) && OffScreen()) {
		Game::DestroyEntityById(GetId());
		return true;
	}

	if (!KillOnOffscreen && OffScreen()) {
		ConfineToScreen();
	}

	return false;
}

bool Entity::OffScreen() const {
	float worldRight = (float)System::GetWindowWidth();
	float worldBottom = (float)System::GetWindowHeight();

	return Right() < 0 || Bottom() < 0 || Left() > worldRight || Top() > worldBottom;
}

void Entity::ConfineToScreen() {
	float width = (float)System::GetWindowWidth();
	float height = (float)System::GetWindowHeight();

	if (Left() < 0) {
		SetLeft(0);
	}
	else if (Right() > width) {
		SetRight(width);
	}
	if (Top() < 0) {
		SetTop(0);
	}
	else if (Bottom() > height) {
		SetBottom(height);
	}
}

void Entity::Draw(SDL_Renderer* renderer) const {
	// "Dead" entities are not drawn
	if (!Alive) return;

	// Check if we have a valid texture
	if (mTex) {
		// Compute rectangle on screen
		SDL_Rect screenRect;
		screenRect.w = mTex->GetWidth();
		screenRect.h = mTex->GetHeight();
		screenRect.x = (int)(Center.x - mTex->GetWidth() / 2);
		screenRect.y = (int)(Center.y - mTex->GetHeight() / 2);

		double angle = (atan2(Velocity.y, Velocity.x) * 180 / M_PI);

		// Draw textured rectangle
		SDL_RenderCopyEx(renderer, mTex->GetSDLTexture(), NULL, &screenRect, angle, NULL, SDL_FLIP_NONE);

		// Draw health bar
		if (mHealthBarVisible) {
			if (mHealth < 25.F) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
			}
			else if (mHealth < 50.0F) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			SDL_RenderFillRect(renderer, &mHealthRect);
		}
	}
	else {
		// Draw a placeholder
		SDL_Rect screenRect;
		screenRect.w = 64;
		screenRect.h = 64;
		screenRect.x = (int)(Center.x - screenRect.w / 2);
		screenRect.y = (int)(Center.y - screenRect.h / 2);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &screenRect);
	}
}

bool Entity::CollidesWith(Entity* ent) const {
	// "Dead" entities do not collide
	if (!Alive || !ent->Alive) return false;

	SDL_Rect r1 = { (int)Left(), (int)Top(), (int)Width(), (int)Height() };
	SDL_Rect r2 = { (int)ent->Left(), (int)ent->Top(), (int)ent->Width(), (int)ent->Height() };

	// This is to quiet the compiler about forcibly converting from SDL_bool to bool
	return SDL_HasIntersection(&r1, &r2) ? true : false;
}

void Entity::Heal(float amount) {
	mHealth += amount;

	if (mHealth >= 100.F) {
		mHealth = 100.F;
	}
}

// Returns true if the hit killed the entity
bool Entity::Hit(float damage) {
	mHealth -= damage;

	if (mHealth <= 0) {
		mHealth = 0;
		Explode();
		return true;
	}

	return false;
}

// Create an explosion at the current location and remove the entity
void Entity::Explode() {
	float scale = mExplosionSize * RandomFloatInclusive(0.85F, 1.15F);
	Explosion* e = new Explosion(Center, Game::ExplosionTex, scale);
	Game::Explosions.push_back(e);

	Destroy();
}

// Remove the entity from the game
void Entity::Destroy() {
	Alive = false;
	Game::DestroyEntityById(GetId());
}

void Entity::Shoot() {
	float spreadX = RandomFloatInclusive(0.9F, 1.1F);
	float spreadY = RandomFloatInclusive(0.9F, 1.1F);

	Missile* missile = new Missile(Center, mTeam == Team::Player ? Game::ShotTex : Game::EnemyShotTex, mTeam);
	missile->Velocity = Vec2(Velocity.x * 2 * spreadX, Velocity.y * 2 * spreadY);
	Game::Entities.push_back(missile);

	// Play missile shot sound
	Mix_PlayChannel(0, Game::ShotSound, 0);
}