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
        entityCapacity{ INITIAL_ENTITY_CAPACITY }
    {
        for (entity_id i = 0; i < entityCapacity; i++)
        {
            freeList[i] = Entity{ i, 0 };
        }
    }

    Entity Scene::createEntity()
    {
        Entity ent = this->nextFree();
        //entityInfoList[ent.id] = EntityInfo{ ent, ComponentMask{} };
        entityCount++;
        //lazyListDirtyFlag = true;
        return ent;
    }

    //const std::vector<Entity>& Scene::getEntities()
    //{
    //    if (lazyListDirtyFlag)
    //    {
    //        entitiesLazyList.clear();
    //        for (entity_id i = 0; i < entityInfoList.size(); i++)
    //        {
    //            if (entityInfoList[i].entity != INVALID_ENTITY)
    //            {
    //                entitiesLazyList.push_back(entityInfoList[i].entity);
    //            }
    //        }
    //        lazyListDirtyFlag = false;
    //    }
    //    return entitiesLazyList;
    //}

    Entity Scene::nextFree()
    {
        if (freeList.size() == 0)
        {
            for (entity_id i = entityCapacity; i < entityCapacity * 2; i++)
            {
                freeList.push_back(Entity {i, 0});
            }
            entityCapacity *= 2;
            //entityInfoList.resize(entityCapacity, INVALID_ENTITY_INFO);
        }
        Entity ent = freeList.front();
        freeList.pop_front();
        return ent;
    }

    bool Scene::isAlive(Entity entity)
    {
      /*  return (entity.id != INVALID_ENTITY_ID
            && entity.id < entityInfoList.size()
            && entity.version == entityInfoList[entity.id].version);*/
        return false;
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (isAlive(entity))
        {
            //entityInfoList[entity.id] = INVALID_ENTITY_INFO;
            entity.version++;
            freeList.push_back(entity);
            entityCount--;
            //lazyListDirtyFlag = true;
        }
    }
}