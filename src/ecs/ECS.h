#pragma once
#include <vector>
#include <array>
#include <bitset>
#include <deque>
#include <stdexcept>
#include <string>
#include "../misc/SparseMap.h"
#include "../misc/Iterators.h"

namespace ECS
{
    // shared consts
    const size_t MAX_COMPONENTS = 32;
    const size_t INITIAL_ENTITY_CAPACITY = 128;

    using entity_id = unsigned int;
    using entity_version = unsigned int;
    using component_id = size_t;
    using component_index = size_t;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    const entity_id INVALID_ENTITY_ID = -1;
    const entity_version INVALID_ENTITY_VERSION = -1;
    const component_index INVALID_COMPONENT_INDEX = -1;

    // public-facing Entity used for indexing for components and systems
    struct Entity
    {
        entity_id id;
        entity_version version;

        /**
         * @brief Returns true if the id and version match.
         * @param other
         * @return 
         */
        bool operator==(const Entity& other) const;
    };

    const Entity INVALID_ENTITY{ INVALID_ENTITY_ID, INVALID_ENTITY_VERSION };

    extern int componentCounter; // defined in ECS.cpp

    /**
     * @brief Initializes the component id for a given Component. The id starts from 0
     * and increases as more components get registered
     * @tparam Component
     * @return id of Component
     */
    template <class Component>
    component_id getComponentId()
    {
        static component_id componentId = componentCounter++;
        return componentId;
    }

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
        virtual void onEntityDestroyed(entity_id id) = 0;
    };

    template <class Component>
    struct ComponentPool : public IComponentPool
    {
        SparseMap<entity_id, component_index> componentMap;
        std::vector<Component> components;
    
        ComponentPool() :
            componentMap( INVALID_ENTITY_ID, INVALID_COMPONENT_INDEX, INITIAL_ENTITY_CAPACITY ),
            components{ }
        { }

        /**
         * @brief Creates a new Component and links it with id if one isn't already linked.
         * This method does not update an entity's componentMask.
         * @param id
         */
        void assign(entity_id id)
        {
            if (componentMap[id] != INVALID_COMPONENT_INDEX) return;
            components.push_back(Component{ });
            componentMap.map(id);
        }

        /**
         * @brief Unlinks id from its Component, if it isn't already linked.
         * This method does not update the entity's componentMask.
         * @param id
         */
        void unassign(entity_id id)
        {
            if (componentMap[id] == INVALID_COMPONENT_INDEX) return;
            components[componentMap[id]] = components.back();
            components.pop_back();
            componentMap.unmap(id);
        }

        void onEntityDestroyed(entity_id id) override
        {
            unassign(id);
        }
    };

    class Scene
    {
        std::vector<IComponentPool*> componentPools;

        // Entity data
        unsigned int entityCapacity = INITIAL_ENTITY_CAPACITY;
        SparseMap<entity_id, entity_id> entityMap;
        std::vector<ComponentMask> componentMasks;
        std::vector<Entity> liveList;
        std::deque<Entity> freeList;

        /**
         * @brief Retrieves the next free Entity from the free list.
         * If we run out of free entities, we will create more
         * before returning the first free entity, doubling
         * the capacity of the free list. This will also double
         * the capacity of the live entities list since we will use it
         * as a map when destroying entities
         * @return next free id
         */
        Entity nextFree();

        /**
         * @brief Tests if this entity is alive
         * @param entity
         * @return true if alive, false if not
         */
        bool isAlive(Entity entity);

        /**
         * @brief Checks if the given Component type has been registered to the manager
         * @tparam Component
         * @return false if the Component has not been registered,
         *		   or if it cannot be registered (component id >= MAX_COMPONENTS)
         */
        template <class Component>
        bool isComponentRegistered()
        {
            component_id id = getComponentId<Component>();
            if (id >= componentPools.size()) return false;
            IComponentPool* pool = componentPools[id];
            if (pool == nullptr) return false;
            return true;
        }

        /**
         * @brief Registers the component to the component manager if it wasn't already.
         * @throws std::out_of_range if the component id >= MAX_COMPONENTS
         * @tparam Component
         */
        template <class Component>
        void registerComponent()
        {
            if (isComponentRegistered<Component>()) return;
            component_id id = getComponentId<Component>();
            if (id >= MAX_COMPONENTS)
            {
                throw std::out_of_range("MAXIMUM COMPONENTS REACHED (" + std::to_string(MAX_COMPONENTS) + ")");
            }
            IComponentPool* pool = new ComponentPool<Component>();
            if (id = componentPools.size())
            {
                componentPools.push_back(pool);
            }
            else
            {
                componentPools.resize(id + 1, nullptr);
                componentPools[id] = pool;
            }
        }

    public:
        Scene();
        ~Scene();

        /**
         * @brief Retrieves an unused entity, or creates a new one if a free
         * entity is not available, and adds it to the live entities list
         * before returning it.
         * @return
         */
        Entity createEntity();

        /**
         * @brief Returns a read-only vector of all current living entities. This list is a LIVE list.
         */
        const std::vector<Entity>& getEntities() const;

        /**
         * @brief If the given entity is in the live entities list, it will
         * be removed, and this entity will be returned to the free list with
         * its version incremented.
         * This method does not deregister components.
         * @param entity
         */
        void destroyEntity(Entity entity);

        /**
         * @brief Calls registerComponent<Component> to ensure the component is registered,
         * then assigns a component to the given entity.
         * @tparam Component
         * @param entityId
         * @throws std::out_of_range if registerComponent fails
         */
        template <class Component>
        void addComponent(Entity entity)
        {
            // make sure this entity is alive
            if (isAlive(entity))
            {
                registerComponent<Component>();
                component_id compId = getComponentId<Component>();
                componentMasks[entityMap[entity.id]].set(compId);
                IComponentPool* pool = componentPools[compId];
                ComponentPool<Component>* cpool = static_cast<ComponentPool<Component>*>(pool);
                cpool->assign(entity.id);
            }
        }

        /**
         * @brief Retrives a pointer to the Component attached to the given entity, if one exists
         * @tparam Component 
         * @param entity Must be a living entity
         * @return nullptr if entity is not alive or if component is not added
         */
        template <class Component>
        Component* getComponent(Entity entity)
        {
            if (!isComponentRegistered<Component>()) return nullptr;
            if (!isAlive(entity)) return nullptr;
            component_id compId = getComponentId<Component>();
            if (componentMasks[entityMap[entity.id]].test(compId))
            {
                IComponentPool* pool = componentPools[compId];
                ComponentPool<Component>* cpool = static_cast<ComponentPool<Component>*>(pool);
                return &(cpool->components[cpool->componentMap[entity.id]]);
            }
            return nullptr;
        }

        /**
         * @brief Unassigns Component from the given entityId
         * @tparam Component
         * @param entityId
         */
        template <class Component>
        void removeComponent(Entity entity)
        {
            if (!isComponentRegistered<Component>()) return;
            if (!isAlive(entity)) return;
            component_id compId = getComponentId<Component>();
            componentMasks[entityMap[entity.id]].reset(compId);
            IComponentPool* pool = componentPools[compId];
            ComponentPool<Component>* cpool = static_cast<ComponentPool<Component>*>(pool);
            cpool->unassign(entity.id);
        }
    };

    template <class... Component>
    struct SceneView
    {
        SceneView(Scene& scene) : scene(&scene)
        {
            if (sizeof...(Component) == 0)
            {
                all = true;
            }
            else
            {
                std::array<component_id> componentIds = {
                    getComponentId<Component>()...
                };

                for (int i = 0; i < componentIds.size(); i++)
                {
                    componentMask.set(componentIds[i]);
                }
            }
        }        

        ForwardIterator<Entity> begin()
        {
            return ForwardIterator<Entity>();
        }

        ForwardIterator<Entity> end()
        {
            return ForwardIterator<Entity>();
        }

    private:
        Scene* scene = nullptr;
        ComponentMask componentMask{ };
        bool all = false;
    };
}