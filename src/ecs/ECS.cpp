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

    EntityManager::EntityManager() : freeIds(INITIAL_ENTITY_CAPACITY),
        idCapacity{ INITIAL_ENTITY_CAPACITY }
    {
        for (size_t i = 0; i < freeIds.size(); i++)
        {
            freeIds[i] = i;
        }
    }

    EntityId EntityManager::nextFreeId()
    {
        if (freeIds.size() == 0)
        {
            for (size_t i = idCapacity; i < idCapacity * 2; i++)
            {
                freeIds.push_back(i);
            }
            idCapacity *= 2;
        }
        EntityId id = freeIds.front();
        freeIds.pop_front();
        return id;
    }

    void EntityManager::freeId(EntityId id)
    {
        freeIds.push_front(id);
    }

    Entity EntityManager::next()
    {
        return Entity{ this->nextFreeId(), ComponentMask{} };
    }

    void EntityManager::free(Entity entity)
    {
        this->freeId(entity.id);
        entity.id = ECS::INVALID_ENTITY_ID;
        entity.componentMask = ComponentMask{};
    }
}