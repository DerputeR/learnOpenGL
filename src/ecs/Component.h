#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"

namespace ECS
{
	/**
	 * @brief Interface for ComponentPools
	 * so we can use polymorphism to call onEntityDestroyed
	 * for all ComponentPools in a vector when an entity is deleted
	 */
	struct IComponentPool
	{
		virtual ~IComponentPool() = default;
		virtual void onEntityDestroyed(EntityID entityId) = 0;
	};

	template <class Component>
	struct ComponentContainer
	{
		EntityID entityId;
		Component component;
	};

	template <class Component>
	class ComponentPool : public IComponentPool
	{
		std::vector<ComponentContainer<Component>> pool;
		std::unordered_map<EntityID, size_t> entityToComponent;
	public:
		ComponentPool() : pool(INIT_ENTITY_CAPCAITY),
			entityToComponent()
		{}

		void addEntity(EntityID entity)
		{
			pool.push_back({entity, Component()});
			entityToComponent.insert(entity, pool.size() - 1);
		}

		void removeEntity(EntityID entity)
		{
			size_t poolIndex = entityToComponent[entity];
			ComponentContainer<Component> back = pool.back();
			pool[poolIndex] = back;
			entityToComponent[back.entityId] = poolIndex;
			pool.pop_back();
			entityToComponent[entity] = -1;
		}

		Component* getComponent(EntityID entity)
		{
			return pool[entity].component;
		}

		void onEntityDestroyed(EntityID entity)
		{
			removeEntity(entity);
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