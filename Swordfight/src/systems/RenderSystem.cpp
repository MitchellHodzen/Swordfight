#include "systems/RenderSystem.h"
#include "kecs/KECS.h"
#include "Components/c_transform.h"
#include "Components/c_rect.h"
#include "Components/c_render.h"
#include "rendering/Texture.h"
#include "rendering/Spritesheet.h"
#include "rendering/Animation.h"
#include "Rectangle.h"
#include "rendering/KRenderer.h"

void RenderSystem::Draw(KRenderer& kRenderer)//std::vector<Entity*>* entityList, std::vector<TextElement*>* textList)//, Camera* camera)
{
	kRenderer.ClearScreen();

	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Transform, Render>();

	for (Entity entity : entities)
	{

		//Most optimal way to do position memory wise? Take another look
		Transform* trans = EntityManager::GetComponent<Transform>(entity);
		Vector2 position = trans->position;
		if (EntityManager::IsValidEntity(trans->parentEntity))
		{
			//If there is a parent entity, add the transform
			Transform* parentTrans = EntityManager::GetComponent<Transform>(trans->parentEntity);
			if (parentTrans != nullptr)
			{
				position = Vector2::ComponentWiseAddition(trans->position, parentTrans->position);
			}
		}
		Render* render = EntityManager::GetComponent<Render>(entity);
		Texture* text = render->spritesheet->GetTexture();
		Rectangle* spriteCutRect = render->spritesheet->GetSprite(render->animationInstance->GetCurrentAnimSprite());

		kRenderer.RenderTexture(*text, position.GetX(), position.GetY(), *spriteCutRect, false);
	}
	kRenderer.DrawScreen();
}

