#pragma once

#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "System.h"
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Random.h"
#include "Team.h"
#include "Spatial.h"

class Game {
	Spatial*				mSpatial;
	TTF_Font*				mFontSmall;
	TTF_Font*				mFontLarge;
	Texture*				mCannonIconTex;

public:
	Game();

	bool				    Initialize();
	void				    Shutdown();

	void				    Update(float dt);
	void				    Draw(SDL_Renderer* renderer);
	void					Reset();

	void				    OnKeyDown(const SDL_KeyboardEvent& kbe);
	void				    OnKeyUp(const SDL_KeyboardEvent& kbe);
	void				    OnMouseDown(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseUp(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseMotion(const SDL_MouseMotionEvent& mme);

	float                   WorldLeft() const { return 0.0f; }
	float                   WorldRight() const { return (float)System::GetWindowWidth(); }
	float                   WorldTop() const { return 0.0f; }
	float                   WorldBottom() const { return (float)System::GetWindowHeight(); }

	// Public globals
	static void				DestroyEntityById(int id);
	static void				DestroyPlayer();

	static SDL_Rect			EnergyRect;

	static Texture*			ShuttleTex;
	static Texture*			ShotTex;
	static Texture*			EnemyShotTex;
	static Texture*			EnemyTex;
	static Texture*			ExplosionTex;

	static Mix_Chunk*		ShotSound;

	static Player*				    MainPlayer;
	static std::vector<Entity*>		Entities;
	static std::vector<Explosion*>	Explosions;
};