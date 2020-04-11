#include "Texture.h"
#include "RenderSystem.h"

Texture::Texture()
{
	sdlTexture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	FreeTexture();
}

bool Texture::LoadTexture(std::string path, RenderSystem* renderSystem)
{
	bool success = true;
	FreeTexture();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image at path: " << path << " SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		sdlTexture = SDL_CreateTextureFromSurface(renderSystem->GetSdlRenderer(), loadedSurface);
		if (sdlTexture == NULL)
		{
			std::cout << "Unable to create texture. SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			width = loadedSurface->w;
			height = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return success;
}

void Texture::FreeTexture()
{
	if (sdlTexture != NULL)
	{
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = NULL;
		width = 0;
		height = 0;
	}
}

int Texture::GetWidth()
{
	return width;
}
int Texture::GetHeight()
{
	return height;
}
SDL_Texture* Texture::GetTexture()
{
	return sdlTexture;
}