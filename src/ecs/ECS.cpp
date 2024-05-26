#include "ECS.h"

namespace ECS
{
    Scene::~Scene()
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

    Scene::Scene() : freeList(INITIAL_ENTITY_CAPACITY),
        entityCapacity{ INITIAL_ENTITY_CAPACITY },
        entityInfoList(INITIAL_ENTITY_CAPACITY, INVALID_ENTITY_INFO)
    {
        for (EntityId i = 0; i < entityCapacity; i++)
        {
            freeList[i] = Entity{ i, 0 };
        }
    }

    Entity Scene::createEntity()
    {
        Entity ent = this->nextFree();
        entityInfoList[ent.id] = EntityInfo{ ent.version, ComponentMask{} };
        entityCount++;
        return ent;
    }

    Entity Scene::nextFree()
    {
        if (freeList.size() == 0)
        {
            for (EntityId i = entityCapacity; i < entityCapacity * 2; i++)
            {
                freeList.push_back(Entity {i, 0});
            }
            entityCapacity *= 2;
            entityInfoList.resize(entityCapacity, INVALID_ENTITY_INFO);
        }
        Entity ent = freeList.front();
        freeList.pop_front();
        return ent;
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (entity != INVALID_ENTITY
            && entity.id < entityInfoList.size()
            && entityInfoList[entity.id].version == entity.version)
        {
            entityInfoList[entity.id] = INVALID_ENTITY_INFO;
            entity.version++;
            freeList.push_back(entity);
            entityCount--;
        }
    }
}