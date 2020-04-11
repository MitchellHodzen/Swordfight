#pragma once
#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class RenderSystem;

class Texture
{
public:
	Texture();
	~Texture();
	bool LoadTexture(std::string path, RenderSystem* renderSystem);
	void FreeTexture();
	int GetWidth();
	int GetHeight();
	SDL_Texture* GetTexture();
private:
	SDL_Texture* sdlTexture;
	int width;
	int height;
};

