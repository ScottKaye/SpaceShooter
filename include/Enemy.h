#pragma once

#include <SDL.h>

#include "Texture.h"
#include "Team.h"
#include "Vec2.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(const Vec2& pos, const Texture* tex);
	
	//Number of frames this enemy has been alive for, since enemies spawn offscreen and shouldn't be instantly destroyed for being out-of-bounds
	bool					Update(float dt);
};