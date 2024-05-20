#pragma once
#include <vector>
#include "Entity.h"
#include "Component.h"

class Scene
{
	std::vector<ECS::Entity> entities;
	std::vector<IComponentPool*> componentPools;
};

