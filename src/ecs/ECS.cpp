#include "ECS.h"
#include <iostream>

namespace ECS
{
	EntityID Scene::createEntity()
	{

		entities.push_back({ entities.size(), ComponentMask{} });
		return entities.back().id;
	}

	ComponentPool::ComponentPool(size_t componentSize)
		: componentSize{ componentSize },
		pool(componentSize * INIT_ENTITY_CAPCAITY)
	{
	}

	Scene::~Scene()
	{
		for (ComponentPool* p : componentPools)
		{
			delete(p);
		}
	}

	void testPools()
	{
		Scene scene;
	}
}