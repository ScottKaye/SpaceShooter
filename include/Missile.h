#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Team.h"
#include "Vec2.h"

class Missile : public Entity {
public:
	Missile(const Vec2& pos, const Texture* tex, Team team);
};