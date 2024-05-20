#pragma once
#include <vector>
#include <bitset>
#include <deque>
#include <stdexcept>
#include <string>

namespace ECS
{
	// shared consts
	const size_t MAX_COMPONENTS = 32;
	const size_t INITIAL_ENTITY_CAPACITY = 128;

	using EntityId = size_t;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	struct Entity
	{
		EntityId id;
		ComponentMask componentMask;
	};

	class EntityManager
	{
		size_t idCapacity;
		std::deque<EntityId> freeIds;
	public:
		EntityManager();
		/**
		 * @brief Retrieves the next free EntityId.
		 * If we run out of ids, we will add a new set of free ids 
		 * before returning the first free id.
		 * @return next free id 
		 */
		EntityId nextFreeId();
		/**
		 * @brief Mark the given id as free.
		 * Be careful not to call this multiple times on the same id,
		 * or the free list will be invalid!
		 * @param id 
		 */
		void freeId(EntityId id);
	};

	/**
	 * @brief Interface common to ComponentPools.
	 * This is so that when an entity is destroyed, we can simply
	 * iterate through the vector of all component pools to locate
	 * and remove the link between that entity and any components it had.
	 */
	class IComponentPool
	{
	public:
		virtual ~IComponentPool() = default;
		virtual void onEntityDestroyed() = 0;
	};

	template <class Component>
	class ComponentPool : public IComponentPool
	{
		std::vector<Component> components;
	public:
		ComponentPool() : components(INITIAL_ENTITY_CAPACITY)
		{ }
	};

	class ComponentManager
	{
		std::vector<IComponentPool*> componentPools;

	public:
		ComponentManager();
		~ComponentManager();

		template <class Component>
		void assign(EntityId entityId)
		{

		}

		int componentCounter = 0;

		template <class Component>
		int getComponentId()
		{
			static int componentId = componentCounter++;
			return componentId;
		}

		template <class Component>
		void registerComponent()
		{
			int id = getComponentId<Component>();
			if (id >= MAX_COMPONENTS)
			{
				throw out_of_range("MAXIMUM COMPONENTS REACHED (" + std::to_string(MAX_COMPONENTS) + ")");
			}
			IComponentPool* pool = componentPools[id];
			if (pool == nullptr)
			{
				componentPools[id] = new ComponentPool<Component>();
			}
		}
	};

	class Scene
	{
		std::vector<Entity> entities;

		ComponentManager componentManager;
		EntityManager entityManager;
	public:
		Scene();
		~Scene();
	};
}