#include "RenderSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Components/c_render.h"
#include "Texture.h"
#include "Spritesheet.h"
#include "Rectangle.h"

RenderSystem::~RenderSystem()
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	sdlRenderer = nullptr;
	sdlWindow = nullptr;
	IMG_Quit();
	SDL_Quit();
}


void RenderSystem::Draw()//std::vector<Entity*>* entityList, std::vector<TextElement*>* textList)//, Camera* camera)
{
	SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_RenderClear(sdlRenderer);

	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Render>();

	for (Entity entity : entities)
	{
		//Render red filled quad 
		//Rect& rect = EntityManager::GetComponent<Rect>(entityIndex);


		Transform* trans = EntityManager::GetComponent<Transform>(entity);
		Render* render = EntityManager::GetComponent<Render>(entity);
		Texture* text = render->spritesheet->GetTexture();
		Rectangle* rect = render->spritesheet->GetSprite(render->animationInstance->GetCurrentAnimSprite());

		SDL_Rect sdlRect{ rect->posX, rect->posY, rect->width, rect->height };
		RenderTexture(text, trans->position.GetX() - rect->width / 2, trans->position.GetY() - rect->height / 2, sdlRect);



		//SDL_Rect fillRect = { pos.x + rect.offsetX, pos.y + rect.offsetY, rect.width, rect.height };
		//SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x00, 0x00, 0xFF);
		//SDL_RenderFillRect(sdlRenderer, &fillRect);

		//->Render(round(entity->GetPosX()), round(entity->GetPosY()), sdlRenderer);

	}
	SDL_RenderPresent(sdlRenderer);
	/*
	SDL_RenderClear(sdlRenderer);
	for (unsigned i = 0; i < entityList->size(); ++i)
	{
		//Render((*entityList)[i]);
		Render(entityList->at(i));
	}

	for (unsigned i = 0; i < textList->size(); ++i)
	{
		textList->at(i)->Render();
	}
	SDL_RenderPresent(sdlRenderer);
	*/
}

void RenderSystem::RenderTexture(Texture* texture, int posX, int posY, SDL_Rect& cutRect, SDL_RendererFlip flip)
{
	//flip defaults to SDL_FLIP_NONE
	
	//If we want the position to be the middle, do this calculation
	//SDL_Rect rect{ posX - cutRect->w / 2, posY - cutRect->h / 2, cutRect->w, cutRect->h };
	
	//If we want the position to be the top left do this calcuation
	SDL_Rect rect{ posX, posY, cutRect.w, cutRect.h };

	//Not a memory leak? Memory is just slowly allocated but has an upper bounds
	SDL_RenderCopyEx(sdlRenderer, texture->GetTexture(), &cutRect, &rect, 0.0, NULL, flip);

}

bool RenderSystem::Initialize(int screenWidth, int screenHeight)
{
	std::cout << "Initializing Renderer..." << std::endl;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized. SDL error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		sdlWindow = SDL_CreateWindow("Platform Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (sdlWindow == NULL)
		{
			std::cout << "SDL Window could not be created. SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN);
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
			if (sdlRenderer == NULL)
			{
				std::cout << "SDL Renderer could not be created. SDL error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //old just white one
				//SDL_SetRenderDrawColor(sdlRenderer, 0xB9, 0xD9, 0xEE, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags)) & imgFlags)
				{
					std::cout << "SDL Img could not be initialized. SDL Img error: " << IMG_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	return success;
}

int RenderSystem::GetScreenWidth()
{
	return screenWidth;
}
int RenderSystem::GetScreenHeight()
{
	return screenHeight;
}

SDL_Renderer* RenderSystem::GetSdlRenderer()
{
	return sdlRenderer;
}