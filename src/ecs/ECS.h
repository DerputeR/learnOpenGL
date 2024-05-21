#pragma once
#include <vector>
#include <bitset>
#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ECS
{
    // shared consts
    const size_t MAX_COMPONENTS = 32;
    const size_t INITIAL_ENTITY_CAPACITY = 128;

    using EntityId = size_t;
    using ComponentId = size_t;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    const EntityId INVALID_ENTITY_ID = -1;
    const ComponentId INVALID_COMPONENT_INDEX = -1;

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
        virtual void onEntityDestroyed(EntityId id) = 0;
    };

    template <class Component>
    class ComponentPool : public IComponentPool
    {
        std::vector<Component> components;
        std::vector<ComponentId> entityToComponent;
        std::vector<EntityId> componentToEntity;
    public:
        ComponentPool() : components(INITIAL_ENTITY_CAPACITY),
            entityToComponent(INITIAL_ENTITY_CAPACITY, INVALID_COMPONENT_INDEX),
            componentToEntity(INITIAL_ENTITY_CAPACITY, INVALID_COMPONENT_INDEX)
        { }

        /**
         * @brief Creates a new Component and links it with id
         * @param id
         */
        void assign(EntityId id)
        {
            ComponentId index = components.size();
            components.push_back(Component{ });
            size_t entityCapacity = components.size();
            if (id >= entityCapacity)
            {
                size_t newSize = entityCapacity * 2;
                while (id >= newSize) // this should almost never happen
                {
                    newSize *= 2;
                }
                entityToComponent.resize(newSize, INVALID_COMPONENT_INDEX);
                componentToEntity.resize(newSize, INVALID_ENTITY_ID);
            }
            entityToComponent[id] = index;
            componentToEntity[index] = id;
        }

        /**
         * @brief Unlinks id from its Component, if it isn't already linked
         * @param id
         */
        void unassign(EntityId id)
        {
            ComponentId index = entityToComponent[id];
            if (index == INVALID_COMPONENT_INDEX) return;

            // we will copy the back to the component slot we want removed,
            // pop the back, then make sure the entity that had the back component
            // now points to the replaced slot, and that the replaced slot is
            // linked back to said entity
            ComponentId backIndex = components.size() - 1;
            EntityId backId = componentToEntity[backIndex];

            components[index] = components[backIndex];
            components.pop_back();

            entityToComponent[id] = INVALID_COMPONENT_INDEX;
            componentToEntity[backIndex] = INVALID_ENTITY_ID;
            componentToEntity[index] = backId;
        }

        void onEntityDestroyed(EntityId id) override
        {
            unassign(id);
        }
    };

    class ComponentManager
    {
        int componentCounter = 0;
        std::vector<IComponentPool*> componentPools;
    public:
        ComponentManager();
        ~ComponentManager();

        /**
         * @brief Calls registerComponent<Component> to ensure the component is registered,
         * then assigns a component to the given entity.
         * @tparam Component
         * @param entityId
         * @throws std::out_of_range if registerComponent fails
         */
        template <class Component>
        void assign(EntityId entityId)
        {
            registerComponent<Component>();
            ComponentId compId = getComponentId<Component>();
            IComponentPool* pool = componentPools[compId];
            ComponentPool<Component>* cpool = static_cast<ComponentPool<Component>*>(pool);
            cpool->assign(entityId);
        }

        /**
         * @brief Unassigns Component from the given entityId
         * @tparam Component
         * @param entityId
         */
        template <class Component>
        void unassign(EntityId entityId)
        {
            if (!isRegistered<Component>()) return;
            ComponentId compId = getComponentId<Component>();
            IComponentPool* pool = componentPools[compId];
            ComponentPool<Component>* cpool = static_cast<ComponentPool<Component>*>(pool);
            cpool->unassign(entityId);
        }

        /**
         * @brief Checks if the given Component type has been registered to the manager
         * @tparam Component
         * @return false if the Component has not been registered,
         *		   or if it cannot be registered (component id >= MAX_COMPONENTS)
         */
        template <class Component>
        bool isRegistered(ComponentId id)
        {
            ComponentId id = getComponentId<Component>();
            if (id >= MAX_COMPONENTS) return false;
            IComponentPool* pool = componentPools[id];
            if (pool == nullptr) return false;
            return true;
        }

        /**
         * @brief Initializes the component id for a given Component. The id starts from 0
         * and increases as more components get registered
         * @tparam Component
         * @return id of Component
         */
        template <class Component>
        ComponentId getComponentId()
        {
            static ComponentId componentId = componentCounter++;
            return componentId;
        }

        /**
         * @brief Registers the component to the component manager if it wasn't already.
         * @throws std::out_of_range if the component id >= MAX_COMPONENTS
         * @tparam Component
         */
        template <class Component>
        void registerComponent()
        {
            if (isRegistered<Component>()) return;
            ComponentId id = getComponentId<Component>();
            if (id >= MAX_COMPONENTS)
            {
                throw std::out_of_range("MAXIMUM COMPONENTS REACHED (" + std::to_string(MAX_COMPONENTS) + ")");
            }
            IComponentPool* pool = new ComponentPool<Component>();
        }
    };

    class Scene
    {
        std::vector<Entity> entities;

        ComponentManager componentManager;
        EntityManager entityManager;
    public:

    };
}