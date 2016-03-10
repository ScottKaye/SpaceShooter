#ifndef MISSILE_H_
#define MISSILE_H_

#include "Entity.h"
#include "System.h"
#include "Texture.h"
#include "Team.h"
#include "Vec2.h"

class Missile : public Entity
{
public:
	Missile(const Vec2& pos, const Texture* tex, Team team);
	~Missile() { std::cout << "~Missile()" << std::endl; }
};

#endif