#include "ECS.h"

namespace ECS
{
	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
		for (auto i : componentPools)
		{
			delete(i);
		}
	}

	EntityManager::EntityManager() : freeIds(INITIAL_ENTITY_CAPACITY),
		idCapacity{ INITIAL_ENTITY_CAPACITY }
	{
		for (size_t i = 0; i < freeIds.size(); i++)
		{
			freeIds[i] = i;
		}
	}

	EntityId EntityManager::nextFreeId()
	{
		if (freeIds.size() == 0)
		{
			for (size_t i = idCapacity; i < idCapacity * 2; i++)
			{
				freeIds.push_back(i);
			}
			idCapacity *= 2;
		}
		EntityId id = freeIds.front();
		freeIds.pop_front();
		return id;
	}

	void EntityManager::freeId(EntityId id)
	{
		freeIds.push_front(id);
	}
}