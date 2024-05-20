#pragma once
#include <bitset>
#include <vector>
#include <iostream>
#include <memory>

namespace ECS
{
	typedef unsigned long long EntityID;
	const int MAX_COMPONENTS = 32;
	const int INIT_ENTITY_CAPCAITY = 128;
	typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	struct Entity
	{
		EntityID id;
		ComponentMask componentMask;
	};

	class ComponentPool
	{
		std::vector<char> pool;
		size_t componentSize;
	public:
		ComponentPool(size_t componentSize);

		template <class T>
		ComponentPool create()
		{
			return ComponentPool{ sizeof(T) };
		}
		
		void* getComponent(size_t index)
		{
			return &pool[index * componentSize];
		}
	};

	static int componentCounter;
	template <class T>
	int getComponentId()
	{
		static int componentId = componentCounter++;
		return componentId;
	}

	class Scene {
		std::vector<Entity> entities;
		std::vector<ComponentPool*> componentPools;

	public:
		EntityID createEntity();

		template <class T>
		T* addComponent(EntityID entityId)
		{
			int componentId = getComponentId<T>();
			if (componentPools.size() <= componentId)
			{
				// very unlikely this will happen, so only reallocate
				// to bare minimum needed
				std::cout << "Adding new component pool..." << std::endl;
				componentPools.resize(componentId + 1, nullptr);
			}
			if (componentPools[componentId] == nullptr)
			{
				componentPools[componentId] = new ComponentPool{ ComponentPool{sizeof(T)} };
			}


			// Looks up the component in the pool, and initializes it with placement new
			// TODO: this is dangerous if the entity count surpasses INIT_ENTITY_CAPACITY as right now we have no mechanism to force the pool itself to resize
			T* pComponent = new (componentPools[componentId]->getComponent(entityId)) T;

			// Set the bit for this component to true and return the created component
			entities[entityId].componentMask.set(componentId);
			return pComponent;
		}

		template <class T>
		T* getComponent(EntityID entityId)
		{
			int componentId = getComponentId<T>();
			if (!entities[componentId].componentMask.test(componentId))
			{
				return nullptr;
			}

			T* pComponent = static_cast<T*>(componentPools[componentId]->getComponent(entityId));
			return pComponent;
		}

		~Scene();
	};

	void testPools();
}

