#pragma once
#include <bitset>
#include <vector>
#include <deque>
#include <iostream>
#include <memory>

namespace ECS
{
	using EntityID = unsigned int;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;
	const int MAX_COMPONENTS = 32;
	const int INIT_ENTITY_CAPCAITY = 128;

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

