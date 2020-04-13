#pragma once
#include <unordered_map>
#include <string>

//Forward declarations
class Texture;
class RenderSystem;
class Spritesheet;

class ResourceManager
{
public:
	enum class TextureKey { Player, Bullet, Enemy, FighterSpritesheet};
	enum class SpritesheetKey { Fighter };

	static ResourceManager& GetInstance();
	bool Initialize(RenderSystem* renderSystem);

	~ResourceManager();
	Texture* GetTexture(TextureKey key);
	Spritesheet* GetSpritesheet(SpritesheetKey key);

private:
	static ResourceManager instance;
	std::unordered_map<TextureKey, Texture*>* texturePointerMap = new std::unordered_map<TextureKey, Texture*>();;
	std::unordered_map<SpritesheetKey, Spritesheet*>* spritesheetPointerMap = new std::unordered_map<SpritesheetKey, Spritesheet*>();;

	ResourceManager() {}
	bool LoadTextures(RenderSystem* renderSystem);
	bool LoadSpritesheets();
	void UnloadTextures();
	void UnloadSpritesheets();
	bool LoadTexture(std::string path, TextureKey key, RenderSystem* renderSystem);
	bool LoadSpritesheet(TextureKey textureKey, int spriteWidth, int spriteHeight, int rows, int columns, int spriteCount, SpritesheetKey spritesheetKey);
};

