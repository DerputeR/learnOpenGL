#pragma once
#include <bitset>
#include <vector>
#include <deque>
#include <iostream>
#include <memory>
#include "Component.h"

namespace ECS
{
	const int MAX_COMPONENTS = 32;
	const int INIT_ENTITY_CAPCAITY = 128;
	using EntityID = size_t;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	struct Entity
	{
		EntityID id;
		ComponentMask componentMask;
	};

	class EntityManager
	{
		std::deque<EntityID> freeEntityIds;
	public:
		EntityManager();
		Entity createEntity();
		void freeEntityId(EntityID id);
	};
}

