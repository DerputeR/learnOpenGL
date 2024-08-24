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

    //IComponent::IComponent(Scene* owningScene, Entity owningEntity)
    //{
    //    this->owningScene = owningScene;
    //    this->owningEntity = owningEntity;
    //}

    //Scene* IComponent::getScene()
    //{
    //    return owningScene;
    //}

    //Entity IComponent::getOwner()
    //{
    //    return owningEntity;
    //}

    Scene::Scene() : freeList(INITIAL_ENTITY_CAPACITY),
        entityCapacity{ INITIAL_ENTITY_CAPACITY },
        entityMap( ECS::INVALID_ENTITY_ID, ECS::INVALID_ENTITY_ID, INITIAL_ENTITY_CAPACITY ) 
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
        componentMasks.push_back({});
        return ent;
    }

    const std::vector<Entity>* Scene::getEntities() const
    {
        return &liveList;
    }

    const std::vector<ComponentMask>* Scene::getComponentMasks() const
    {
        return &componentMasks;
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

    bool Scene::isAlive(Entity entity) const
    {
        return (entityMap[entity.id] != INVALID_ENTITY_ID
            && entity.version == liveList[entityMap[entity.id]].version);
    }

    void Scene::destroyEntity(Entity entity)
    {
        if (isAlive(entity))
        {
            // remove from live list
            liveList[entityMap[entity.id]] = liveList.back();
            liveList.pop_back();

            // remove components from component pool
            ComponentMask mask = componentMasks[entityMap[entity.id]];
            for (int i = 0; i < MAX_COMPONENTS; i++)
            {
                if (mask.test(i))
                {
                    componentPools[i]->onEntityDestroyed(entity.id);
                }
            }

            // remove associated component mask
            componentMasks[entityMap[entity.id]] = componentMasks.back();
            componentMasks.pop_back();

            entityMap.unmap(entity.id);

            entity.version++;
            freeList.push_back(entity);
        }
    }

    EntityIterator::EntityIterator(Scene* scene, bool all, ComponentMask mask, size_t startIndex)
        : scene(scene),
        all(all),
        mask(mask),
        entities((scene->getEntities())),
        entityMasks((scene->getComponentMasks())),
        index(startIndex)
    { }

    Entity EntityIterator::operator*() const
    {
        return (*entities)[index];
    }

    void EntityIterator::gotoFirstValid()
    {
        while (index < entityMasks->size() && !isValidIndex(index))
        {
            index += 1;
        }
    }

    EntityIterator& EntityIterator::operator++()
    {
        do {
            index += 1;
        } while (index < entityMasks->size() && !isValidIndex(index));
        return *this;
    }

    EntityIterator EntityIterator::operator++(int)
    {
        EntityIterator beforeInc = *this;
        ++(*this);
        return beforeInc;
    }

    bool EntityIterator::operator==(const EntityIterator& b) const
    {
        return index == b.index
            && scene == b.scene
            && mask == b.mask;
    }

    bool EntityIterator::operator!=(const EntityIterator& b) const
    {
        return !(*this == b);
    }

    bool EntityIterator::isValidIndex(size_t index)
    {
        return index < entityMasks->size() && (all || (mask & (*entityMasks)[index]) == mask);
    }
}