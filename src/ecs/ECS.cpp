#include "ECS.h"

namespace ECS
{
    int componentCounter = 0;

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
        entityMap{ ECS::INVALID_ENTITY_ID, ECS::INVALID_ENTITY_ID } 
    {
        for (entity_id i = 0; i < entityCapacity; i++)
        {
            freeList[i] = Entity{ i, 0 };
        }
    }

    Entity Scene::createEntity()
    {
        Entity ent = this->nextFree();
        entityMap.map(ent.id);
        liveList.push_back(ent);
        return ent;
    }

    const std::vector<Entity>& Scene::getEntities() const
    {
        return liveList;
    }

    Entity Scene::nextFree()
    {
        if (freeList.size() == 0)
        {
            for (entity_id i = entityCapacity; i < entityCapacity * 2; i++)
            {
                freeList.push_back(Entity {i, 0});
            }
            entityCapacity *= 2;
        }
        Entity ent = freeList.front();
        freeList.pop_front();
        return ent;
    }

    bool Scene::isAlive(Entity entity)
    {
        return (entity.id != INVALID_ENTITY_ID
            && entity.id < entityMap.sparseMap.size()
            && entity.version == liveList[entityMap.sparseMap[entity.id]].version);
        return false;
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (isAlive(entity))
        {
            entityMap.unmap(entity.id);
            liveList[entity.id] = liveList.back();
            liveList.pop_back();
            entity.version++;
            freeList.push_back(entity);
        }
    }
}