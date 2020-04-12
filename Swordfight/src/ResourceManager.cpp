#include "ResourceManager.h"
#include "Texture.h"
#include "RenderSystem.h"

ResourceManager::~ResourceManager()
{
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
	return LoadTextures(renderSystem);
}

bool ResourceManager::LoadTextures(RenderSystem* renderSystem)
{

	return LoadTexture("./resources/sprites/ship1.png", TextureKey::Player, renderSystem)
	&& LoadTexture("./resources/sprites/bullet1.png", TextureKey::Bullet, renderSystem)
	&& LoadTexture("./resources/sprites/enemy1.png", TextureKey::Enemy, renderSystem);

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

Texture* ResourceManager::GetTexture(TextureKey key)
{
	if (texturePointerMap->count(key))
	{
		return texturePointerMap->at(key);
	}
	return nullptr;
}

void ResourceManager::UnloadTextures()
{
	texturePointerMap->clear();
}
