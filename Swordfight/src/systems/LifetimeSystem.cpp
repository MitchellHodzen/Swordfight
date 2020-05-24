#include "systems/LifetimeSystem.h"
#include "kecs/KECS.h"
#include "Components/c_lifetime.h"
#include "Tags.h"

void LifetimeSystem::CheckEntityLifetimes()
{
	std::vector<Entity> entities = EntityManager::GetEntitiesWithComponent<Lifetime>();
	std::vector<Entity> entitiesToDestroy;
	for (Entity entity : entities)
	{
		Lifetime* lifetime = EntityManager::GetComponent<Lifetime>(entity);
		uint32_t elapsedTimeMs = lifetime->lifetimeTimer.GetTimeElapsedMs();
		if (elapsedTimeMs > lifetime->timeToLiveMs)
		{
			entitiesToDestroy.push_back(entity);
		}
	}

	for (Entity entityToDestroy : entitiesToDestroy)
	{
		//EntityManager::DestroyEntity(entityToDestroy);
		//Add destroy tag to be picked up by another system
		EntityManager::AddTag<Destroy>(entityToDestroy);
	}
}