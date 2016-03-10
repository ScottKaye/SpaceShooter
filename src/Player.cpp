#include <iostream>

#include "Constants.h"
#include "Game.h"
#include "Player.h"
#include "System.h"

// Resolve externs
SDL_Rect Game::EnergyRect;

const float MOVEMENT_FRICTION = 0.9F;

Player::Player(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex, Team::Player)
	, mSpeed(50)
	, mSpread(0.05F)
	, mCannons(3)
	, mEnergy(100)
	, mMissileSpeed(400)
{
	SetExplosionSize(2);
	SetHealthBarVisibile(true);
	KillOnOffscreen = false;
}

bool Player::Update(float dt) {
	if (!Alive) return false;

	// Health bar
	SDL_Rect* rect = HealthRect();
	rect->x = 5;
	rect->y = 30;
	rect->w = (int)clamp(Health() / 100.F * System::GetWindowWidth(), 0.F, System::GetWindowWidth() - 10.F);
	rect->h = 5;

	const Uint8* keys = System::GetKeyStates();
	bool drain = false;

	// It's not the length of the vector that counts, it's how you apply the force
	if (keys[SDL_SCANCODE_D]) {
		Velocity.x += mSpeed * dt;
		drain = true;
	}
	if (keys[SDL_SCANCODE_A]) {
		Velocity.x -= mSpeed * dt;
		drain = true;
	}
	if (keys[SDL_SCANCODE_W]) {
		Velocity.y -= mSpeed * dt;
		drain = true;
	}
	if (keys[SDL_SCANCODE_S]) {
		Velocity.y += mSpeed * dt;
		drain = true;
	}

	ConfineToScreen();

	// This isn't a true vacuum, the ship will stop eventually
	// Player can barely move if they're out of energy
	// Increase the final divisor to make the ship's slowdown take effect nearer to the end of energy
	float slow = ENERGY_MINIMUM / mEnergy / 3;
	Velocity.x *= MOVEMENT_FRICTION - slow;
	Velocity.y *= MOVEMENT_FRICTION - slow;

	Center.x += Velocity.x;
	Center.y += Velocity.y;

	// Finally handle shooting
	if (keys[SDL_SCANCODE_SPACE]) {
		Shoot();
	}
	else {
		// Restore energy while not shooting
		RestoreEnergy(ENERGY_RESTORE_PASSIVE);
	}

	// If the player moved during this frame, drain some energy
	if (drain) {
		DrainEnergy(ENERGY_DRAIN_THRUSTERS + ENERGY_RESTORE_PASSIVE);
	}

	return false;
}

void Player::Shoot() {
	Uint64 frameNo = System::GetFrameNumber();

	// Energy drains regardless of jam
	DrainEnergy(ENERGY_DRAIN_SPINUP);

	if (!CannonJammed() && frameNo % MISSILE_SPARSITY == 0) {
		// How fast cannons rotate around player
		float spin = (float)frameNo / 50.F;
		double delta, vx, vy;

		for (short i = 0; i++ < Cannons();) {
			float rand = RandomFloatInclusive(0, Spread());

			// Get location of equal amount of points around circle + some spin
			delta = 2 * M_PI * (i + spin + rand) / Cannons();
			vx = sin(delta) * MissileSpeed();
			vy = cos(delta) * MissileSpeed();

			Missile* missile = new Missile(Center, Game::ShotTex, Team::Player);
			missile->Velocity = Vec2((float)vx, (float)vy);
			Game::Entities.push_back(missile);

			// Play missile shot sound
			// First channel (0) is for enemy shots, which come much more infrequently than player shots, heh
			Mix_PlayChannel(frameNo % 63 + 1, Game::ShotSound, 0);

			// Each missile damages the cannon slightly
			DrainEnergy(ENERGY_DRAIN_PER_BULLET);
		}
	}
}

void Player::DrainEnergy(float amount) {
	if (NODRAIN) return;

	mEnergy -= amount;
	mEnergy = mEnergy > ENERGY_MINIMUM ? mEnergy : ENERGY_MINIMUM;

	Game::EnergyRect.w = (int)((System::GetWindowWidth() - 10) * mEnergy / 100);
}

void Player::RestoreEnergy(float amount) {
	mEnergy += amount;
	mEnergy = mEnergy < 100 ? mEnergy : 100;

	Game::EnergyRect.w = (int)((System::GetWindowWidth() - 10) * mEnergy / 100);
}

void Player::Destroy() {
	Alive = false;
	SetHealth(0);
	Game::DestroyPlayer();
}