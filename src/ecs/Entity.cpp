#include "Entity.h"
#include <iostream>

namespace ECS
{
	EntityManager::EntityManager() : freeEntityIds(INIT_ENTITY_CAPCAITY, 0)
	{
		for (size_t i = 0; i < freeEntityIds.size(); i++)
		{
			freeEntityIds[i] = i;
		}
	}

	Entity EntityManager::createEntity()
	{
		EntityID id = freeEntityIds.front();
		freeEntityIds.pop_front();
		return Entity{ id, ComponentMask{ } };
	}

	void EntityManager::freeEntityId(EntityID id)
	{
		freeEntityIds.push_front(id);
	}
}