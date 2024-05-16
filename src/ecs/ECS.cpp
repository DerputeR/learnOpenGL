#include "ECS.h"

ECS::EntityID ECS::Scene::createEntity()
{
	entities.push_back({ entities.size(), ComponentMask() });
	return entities.back().id;
}