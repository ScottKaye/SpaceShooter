#include <iostream>
#include <string>

#include "Constants.h"
#include "Game.h"
#include "Text.h"

//resolve externs
Texture*				Game::ShuttleTex;
Texture*				Game::ShotTex;
Texture*				Game::EnemyShotTex;
Texture*				Game::EnemyTex;
Texture*				Game::ExplosionTex;
std::vector<Entity*>	Game::Entities;
std::vector<Explosion*>	Game::Explosions;
Player*					Game::MainPlayer;

//private globals
bool showHitboxes = false;
Text* energyText;
float enemySpawnChance = 0.01F;

Game::Game()
{ }

bool Game::Initialize()
{
	std::cout << "Initializing game" << std::endl;

	// set some window properties
	System::SetWindowSize(480, 640);
	System::SetWindowTitle("Space Hell");

	// get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	// load all textures
	ShuttleTex = Texture::Load("media/shuttle.png", renderer);
	ShotTex = Texture::Load("media/shot.png", renderer);
	EnemyShotTex = Texture::Load("media/shot2.png", renderer);
	EnemyTex = Texture::Load("media/alien.png", renderer);
	ExplosionTex = Texture::Load("media/explosion.tga", renderer);
	mCannonIconTex = Texture::Load("media/cannon-icon.png", renderer);

	// create spatial hash map
	mSpatial = new Spatial(System::GetWindowWidth(), System::GetWindowHeight(), 50);

	// spawn player
	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.5f * System::GetWindowHeight();
	MainPlayer = new Player(spawnPos, ShuttleTex);
	MainPlayer->Heal(100);
	MainPlayer->RestoreEnergy(100);
	MainPlayer->SetCannons(3);

	Entities.push_back(MainPlayer);

	//Load sounds
	ShotSound = Mix_LoadWAV("media/shot.wav");

	//Load fonts and UI
	mFontSmall = TTF_OpenFont("media/VISITOR.FON", 0);
	mFontLarge = TTF_OpenFont("media/visitor1.ttf", 28);

	//create energy stuff
	EnergyRect = { 5, 5, (int)WorldRight() - 10, 20 };
	//This text is drawn every single frame, so don't recreate the texture each time
	energyText = new Text("Energy", Vec2(8, 2), mFontLarge, { 80, 150, 255 }, renderer);

	return true;
}

void Game::Shutdown()
{
	std::cout << "Shutting down game" << std::endl;

	// delete entities
	for (unsigned i = 0; i < Entities.size(); i++) {
		delete Entities[i];
	}
	Entities.clear();

	// release audio
	Mix_FreeChunk(ShotSound);

	// close fonts
	TTF_CloseFont(mFontSmall);
	TTF_CloseFont(mFontLarge);

	// delete spatial hash map
	mSpatial->Destroy();

	// destroy all textures
	Texture::Destroy(ShuttleTex);
	Texture::Destroy(ShotTex);
	Texture::Destroy(EnemyShotTex);
	Texture::Destroy(EnemyTex);
	Texture::Destroy(ExplosionTex);
	Texture::Destroy(mCannonIconTex);
	delete energyText;
}

void Game::Draw(SDL_Renderer* renderer)
{
	// clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//
	// Background
	// Effects, debug stuff
	//

	//drawing the actual spatial boundaries makes a cool grid effect, I think I'll keep it
	int size = mSpatial->Cellsize();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 32);
	for (int col = 0; col < mSpatial->Cols(); ++col) {
		for (int row = 0; row < mSpatial->Rows(); ++row) {
			SDL_Rect rect = { col * size, row * size, size, size };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	//hitboxes
	if (showHitboxes) {
		//all entity hitboxes

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (unsigned i = 0; i < Entities.size(); ++i) {
			SDL_Rect rect = { (int)Entities[i]->Left(), (int)Entities[i]->Top(), (int)Entities[i]->Height(), (int)Entities[i]->Width() };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	//
	// Entities
	// Missiles, enemies, etc
	//

	// draw explosions
	for (unsigned i = 0; i < Explosions.size(); ++i) {
		Explosions[i]->Draw(renderer);
	}

	// draw entities
	for (unsigned i = 0; i < Entities.size(); ++i) {
		Entities[i]->Draw(renderer);
	}

	//
	// Top level
	// UI
	//

	// draw energy bar
	SDL_SetRenderDrawColor(renderer, 50, 128, 225, 128);
	SDL_RenderFillRect(renderer, &EnergyRect);

	//Text
	energyText->Draw(renderer);
	Text::InstantDraw(std::to_string(MainPlayer->Cannons()), Vec2(50, (float)System::GetWindowHeight() - 28), mFontLarge, { 255, 255, 255 }, renderer);

	//"Number of cannons" icon
	SDL_Rect cannonRect = { 10, System::GetWindowHeight() - 28, 28, 28 };
	SDL_RenderCopy(renderer, mCannonIconTex->GetSDLTexture(), NULL, &cannonRect);
}

void Game::Update(float dt)
{
	// reset spatial hashmap
	mSpatial->ClearCells();
	mSpatial->Register(MainPlayer);

	// register entities
	for (unsigned i = 0; i < Entities.size(); ++i) {
		Entity* e = Entities[i];

		// "garbage collector", clean up and move on
		if (e->Garbage) {
			delete Entities[i];
			Entities[i] = Entities.back();
			Entities.pop_back();

			continue;
		}

		// if entity has been killed by update, move on
		if (e->Update(dt)) continue;

		mSpatial->Register(e);

		//collisions
		std::vector<Entity*> nearby = mSpatial->GetNearby(e);
		for (unsigned j = 0; j < nearby.size(); ++j) {
			Entity* e2 = nearby[j];

			//Do not collide with self or same team
			if (e == e2 || e->GetTeam() == e2->GetTeam()) continue;

			//Are we colliding with anything?
			if (e->CollidesWith(e2)) {
				float e_dmg = e->CollisionDamage;
				float e2_dmg = e2->CollisionDamage;

				if (e2->Hit(e_dmg)) {
					mSpatial->Unregister(e2);
				}

				if (e->Hit(e2_dmg)) {
					mSpatial->Unregister(e);
				}

				break;
			}
		}
	}

	// update explosions
	for (unsigned i = 0; i < Explosions.size(); i++) {
		Explosion* e = Explosions[i];
		e->Update(dt);

		// remove the explosion if it is finished
		if (e->IsFinished()) {
			delete e;
			Explosions[i] = Explosions.back();
			Explosions.pop_back();
			continue;
		}
	}

	//Game events
	//Should we spawn an enemy?
	if (RandomFloatInclusive(0, 1) <= enemySpawnChance) {
		//Random position away from center screen, offscreen
		float worldAngle = RandomFloatInclusive(0, 1) * (float)M_PI * 2.F;

		float maxWorldDimension = std::fmaxf(WorldRight(), WorldBottom());
		maxWorldDimension = 400;

		float x = sin(worldAngle) * maxWorldDimension + WorldRight() / 2.F;
		float y = cos(worldAngle) * maxWorldDimension + WorldBottom() / 2.F;

		//slope
		//if player is alive, move towards player
		//otherwise aim towards a random point on the map
		Vec2 target = MainPlayer->Alive
			? MainPlayer->Center
			: Vec2(RandomFloatInclusive(0, WorldRight()), RandomFloatInclusive(0, WorldBottom()));

		float vx = (target.x - x) / 2;
		float vy = (target.y - y) / 2;

		//Random speed
		float factor = RandomIntInclusive(100, 200) / sqrt(vx * vx + vy * vy);
		vx *= factor;
		vy *= factor;

		Enemy* enemy = new Enemy(Vec2(x, y), EnemyTex);
		enemy->Velocity = Vec2(vx, vy);
		Entities.push_back(enemy);
	}
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		System::Quit();
		break;
	case SDLK_h:
		if (MainPlayer->Health() < 100 && MainPlayer->Energy() > 50) {
			std::cout << "heal" << std::endl;
			MainPlayer->Heal(25);
			MainPlayer->DrainEnergy(50);
		}
		break;
	case SDLK_UP:
		if (MainPlayer->Energy() == 100) {
			MainPlayer->DrainEnergy(100);
			MainPlayer->AddCannon();
		}
		break;
	case SDLK_DOWN:
		if (MainPlayer->Cannons() > 1) {
			MainPlayer->RemoveCannon();
			MainPlayer->RestoreEnergy(50);
		}
		break;
	case SDLK_r:
		Reset();
		break;
	case SDLK_b:
		showHitboxes = !showHitboxes;
		break;
	}
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme)
{
}

void Game::OnWindowResized(int w, int h)
{
	std::cout << "Window resized to " << w << 'x' << h << std::endl;
}

void Game::DestroyEntityById(int id) {
	for (unsigned i = 0; i < Entities.size(); ++i) {
		if (Entities[i]->GetId() == id) {
			Entities[i]->Garbage = true;
			break;
		}
	}
}

void Game::DestroyPlayer() {
	MainPlayer->Alive = false;
}

void Game::Reset() {
	for (unsigned i = 0; i < Entities.size(); ++i) delete Entities[i];
	Entities.clear();

	delete energyText;

	Initialize();
}