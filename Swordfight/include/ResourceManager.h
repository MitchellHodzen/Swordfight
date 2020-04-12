#pragma once
#include <unordered_map>
#include <string>

//Forward declarations
class Texture;
class RenderSystem;

class ResourceManager
{
public:
	enum class TextureKey { Player, Bullet, Enemy};

	static ResourceManager& GetInstance();
	bool Initialize(RenderSystem* renderSystem);

	~ResourceManager();
	Texture* GetTexture(TextureKey key);

private:
	static ResourceManager instance;
	std::unordered_map<TextureKey, Texture*>* texturePointerMap = new std::unordered_map<TextureKey, Texture*>();;

	ResourceManager() {}
	bool LoadTextures(RenderSystem* renderSystem);
	void UnloadTextures();
	bool LoadTexture(std::string path, TextureKey key, RenderSystem* renderSystem);
};

