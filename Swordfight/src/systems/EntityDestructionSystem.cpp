#include "systems/EntityDestructionSystem.h"
#include "kecs/KECS.h"
#include "Components/c_lifetime.h"
#include "Tags.h"

void EntityDestructionSystem::DestroyMarkedEntities()
{
	//Destroy everything with the Destroy tag
	std::vector<Entity> entities = EntityManager::GetEntitiesWithTag<Destroy>();
	for (Entity entity : entities)
	{
		EntityManager::DestroyEntity(entity);
	}
}