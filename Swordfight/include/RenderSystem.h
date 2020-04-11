#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Texture;

class RenderSystem
{
public:
	RenderSystem(int screenWidth, int screenHeight);
	~RenderSystem();
	void Render();
	void Draw();
	bool Initialize(int screenWidth, int screenHeight);
	int GetScreenWidth();
	int GetScreenHeight();
	SDL_Renderer* GetSdlRenderer();

private:
	void RenderTexture(Texture* texture, int posX, int posY, SDL_Rect& cutRect, SDL_RendererFlip flip = SDL_FLIP_NONE);

	SDL_Renderer* sdlRenderer = nullptr;
	SDL_Window* sdlWindow = nullptr;
	int screenWidth;
	int screenHeight;
};

