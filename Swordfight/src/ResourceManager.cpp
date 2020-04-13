#include "ResourceManager.h"
#include "Texture.h"
#include "Spritesheet.h"
#include "RenderSystem.h"

ResourceManager::~ResourceManager()
{
	UnloadSpritesheets();
	UnloadTextures();
}

ResourceManager& ResourceManager::GetInstance()
{
	//No need to check if the instance exists, C++ won't create another static instance
	//Also thread safe by default, C++ automatically locks on instance creation
	static ResourceManager instance;
	return instance;
}

bool ResourceManager::Initialize(RenderSystem* renderSystem) {
	return LoadTextures(renderSystem) && LoadSpritesheets();
}

bool ResourceManager::LoadTextures(RenderSystem* renderSystem)
{

	return LoadTexture("./resources/sprites/ship1.png", TextureKey::Player, renderSystem)
	&& LoadTexture("./resources/sprites/bullet1.png", TextureKey::Bullet, renderSystem)
	&& LoadTexture("./resources/sprites/enemy1.png", TextureKey::Enemy, renderSystem)
	&& LoadTexture("./resources/sprites/SwordfightCharacterSpritesheet.png", TextureKey::FighterSpritesheet, renderSystem);

}

bool ResourceManager::LoadSpritesheets()
{
	std::cout<<"Loading spritesheets"<<std::endl;
	return LoadSpritesheet(TextureKey::FighterSpritesheet, 310, 249, 5, 4, 18, SpritesheetKey::Fighter);
}

bool ResourceManager::LoadTexture(std::string path, TextureKey key, RenderSystem* renderSystem)
{
	std::cout << "Loading Texture at " << path << std::endl;
	Texture* texture = new Texture();
	bool success = texture->LoadTexture(path, renderSystem);
	if (success)
	{
		texturePointerMap->insert_or_assign(key, texture);
	}
	return success;
}

bool ResourceManager::LoadSpritesheet(TextureKey textureKey, int spriteWidth, int spriteHeight, int rows, int columns, int spriteCount, SpritesheetKey spritesheetKey)
{
	Spritesheet* spritesheet = new Spritesheet();
	bool success = spritesheet->GenerateSpritesheet(GetTexture(textureKey), spriteWidth, spriteHeight, rows, columns, spriteCount);
	if (success)
	{
		spritesheetPointerMap->insert_or_assign(spritesheetKey, spritesheet);
	}
	return success;
}

Texture* ResourceManager::GetTexture(TextureKey key)
{
	if (texturePointerMap->count(key))
	{
		return texturePointerMap->at(key);
	}
	return nullptr;
}

Spritesheet* ResourceManager::GetSpritesheet(SpritesheetKey key)
{
	if (spritesheetPointerMap->count(key))
	{
		return spritesheetPointerMap->at(key);
	}
	return nullptr;
}

void ResourceManager::UnloadTextures()
{
	texturePointerMap->clear();
}

void ResourceManager::UnloadSpritesheets()
{
	spritesheetPointerMap->clear();
}
