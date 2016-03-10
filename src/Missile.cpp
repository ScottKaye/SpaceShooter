#include <iostream>

#include "Game.h"
#include "Missile.h"

Missile::Missile(const Vec2& pos, const Texture* tex, Team team)
	: Entity(pos, tex, team)
{
	SetExplosionSize(0.5F);
	SetHealth(1);
	SetHealthBarVisibile(false);
	CollisionDamage = 10;
}