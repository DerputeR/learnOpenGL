#pragma once
#include "ECS.h"
#include <vector>
#include <memory>
#include <string>

namespace ECS
{
    class SceneHierarchy
    {
        std::vector<std::vector<Entity>> tree;

    };
}