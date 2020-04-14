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

	return LoadTexture("./resources/sprites/ship1.png", "Ship", TextureKey::Player, renderSystem)
	&& LoadTexture("./resources/sprites/bullet1.png", "Bullet", TextureKey::Bullet, renderSystem)
	&& LoadTexture("./resources/sprites/enemy1.png", "Enemy", TextureKey::Enemy, renderSystem)
	&& LoadTexture("./resources/sprites/SwordfightCharacterSpritesheet.png", "Fighter Spritesheet", TextureKey::FighterSpritesheet, renderSystem);

}

bool ResourceManager::LoadSpritesheets()
{
	std::cout<<"Loading spritesheets"<<std::endl;
	return LoadSpritesheet(TextureKey::FighterSpritesheet, "Fighter", 310, 249, 5, 4, 18, SpritesheetKey::Fighter);
}

bool ResourceManager::LoadTexture(std::string path, std::string name, TextureKey key, RenderSystem* renderSystem)
{
	std::cout << "Loading " << name << " texture at " << path << std::endl;
	Texture* texture = new Texture();
	bool success = texture->LoadTexture(path, name, renderSystem);
	if (success)
	{
		texturePointerMap->insert_or_assign(key, texture);
	}
	return success;
}

bool ResourceManager::LoadSpritesheet(TextureKey textureKey, std::string name, int spriteWidth, int spriteHeight, int rows, int columns, int spriteCount, SpritesheetKey spritesheetKey)
{
	std::cout<< "Loading " << name <<" spritesheet "<<std::endl;
	Spritesheet* spritesheet = new Spritesheet();
	Texture* texture = GetTexture(textureKey);
	if (texture != nullptr)
	{
		std::cout<< "\tAssigning spritesheet " << name << " texture " << texture->GetName() <<std::endl;
		if (spritesheet->GenerateSpritesheet(GetTexture(textureKey), name, spriteWidth, spriteHeight, rows, columns, spriteCount))
		{
			spritesheetPointerMap->insert_or_assign(spritesheetKey, spritesheet);
			return true;
		}
		else
		{
			std::cout<<"Could not generate spritesheet " << name << std::endl;
			return false;
		}
		
	}
	else
	{
		std::cout<<"No texture found for spritesheet " << name << std::endl;
		return false;
	}
	
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
