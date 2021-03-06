#pragma once

#include <string>
#include <SDL.h>

class Texture {
	SDL_Texture*        mSDLTex;
	int                 mWidth;
	int                 mHeight;

	Texture(SDL_Texture* tex, int width, int height);
	~Texture();

public:
	static Texture*     Load(const std::string& path, SDL_Renderer* renderer);
	static void         Destroy(Texture* tex);

	SDL_Texture*        GetSDLTexture() const { return mSDLTex; }

	int                 GetWidth() const { return mWidth; }
	int                 GetHeight() const { return mHeight; }
};