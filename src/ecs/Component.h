#pragma once
#include <vector>

namespace ECS
{
	struct IComponentPool
	{
		virtual ~IComponentPool() = default;
	};

	template <class Component>
	class ComponentPool : public IComponentPool
	{
		std::vector<Component> pool;
	public:
		ComponentPool() : pool(INIT_ENTITY_CAPCAITY) {}
		Component* getComponent(size_t index)
		{
			return &pool[index];
		}
	};

	class ComponentManager
	{
		int componentCounter;
	public:
		template <class T>
		int getComponentId()
		{
			static int componentId = componentCounter++;
			return componentId;
		}
	};
}