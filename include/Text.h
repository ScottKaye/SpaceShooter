#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "System.h"
#include "Game.h"
#include "Vec2.h"

class Text {
	SDL_Texture*			mTex;
	SDL_Rect				mRect;

public:
	Text(std::string str, Vec2 pos, TTF_Font* font, SDL_Color col, SDL_Renderer* renderer) {
		SDL_Surface* surf = TTF_RenderText_Solid(font, str.c_str(), col);
		mTex = SDL_CreateTextureFromSurface(renderer, surf);

		mRect.w = surf->w;
		mRect.h = surf->h;
		mRect.x = (int)pos.x;
		mRect.y = (int)pos.y;

		SDL_FreeSurface(surf);
	}

	void Draw(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, mTex, NULL, &mRect);
	}

	// Creates, draws, and cleans up a text element
	static void InstantDraw(std::string str, Vec2 pos, TTF_Font* font, SDL_Color col, SDL_Renderer* renderer) {
		Text* t = new Text(str, pos, font, col, renderer);
		t->Draw(renderer);
		delete t;
	}

	~Text() {
		if (mTex) {
			SDL_DestroyTexture(mTex);
		}
	}
};