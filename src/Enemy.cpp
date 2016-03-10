#include <iostream>

#include "Constants.h"
#include "Game.h"
#include "Enemy.h"
#include "System.h"

Enemy::Enemy(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex, Team::Enemy)
{
	SetExplosionSize(1.F);
	SetHealthBarVisibile(true);
}

bool Enemy::Update(float dt) {
	if (Entity::Update(dt)) return true;

	//Randomly shoot
	if (RandomFloatInclusive(0, 1) < ENEMY_SHOOT_CHANCE) {
		Shoot();
	}

	return false;
}