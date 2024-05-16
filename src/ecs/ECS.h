#pragma once
#include <bitset>
#include <vector>

namespace ECS
{
	typedef unsigned long long EntityID;
	const int MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	namespace ComponentManager
	{
		static int componentCounter;
		template <class T>
		int GetId()
		{
			static int componentId = componentCounter++;
			return componentId;
		}
	}

	struct Entity
	{
		EntityID id;
		ComponentMask componentMask;
	};

	class Scene {
		std::vector<Entity> entities;

	public:
		EntityID createEntity();
	};
}

