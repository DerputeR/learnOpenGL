#include "ECS.h"

namespace ECS
{
    ComponentManager::ComponentManager()
    {

    }

    ComponentManager::~ComponentManager()
    {
        for (auto i : componentPools)
        {
            delete(i);
        }
    }

    bool Entity::operator==(const Entity& other) const
    {
        return (id == other.id) && (version == other.version);
    }

    EntityManager::EntityManager() : freeList(INITIAL_ENTITY_CAPACITY),
        entityCapacity{ INITIAL_ENTITY_CAPACITY },
        entities(INITIAL_ENTITY_CAPACITY, INVALID_ENTITY)
    {
        for (EntityId i = 0; i < entityCapacity; i++)
        {
            freeList[i] = Entity{ i, 0 };
        }
    }

    Entity EntityManager::createEntity()
    {
        Entity ent = this->nextFree();
        entities[ent.id] = ent;
        entityCount++;
        return ent;
    }

    Entity EntityManager::nextFree()
    {
        if (freeList.size() == 0)
        {
            for (EntityId i = entityCapacity; i < entityCapacity * 2; i++)
            {
                freeList.push_back(Entity {i, 0});
            }
            entityCapacity *= 2;
            entities.resize(entityCapacity, INVALID_ENTITY);
        }
        Entity ent = freeList.front();
        freeList.pop_front();
        return ent;
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        if (entities[entity.id] == entity)
        {
            entities[entity.id] = INVALID_ENTITY;
            entity.version++;
            entity.componentMask.reset();
            freeList.push_back(entity);
            entityCount--;
        }
    }
}