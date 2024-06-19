#pragma once
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace ECS::Components
{
    struct Transform
    {
        using transform_id = unsigned int;
        const static transform_id NULL_TRANSFORM = -1;

        transform_id parent_id;
        // global transforms
        glm::vec3 position{ 0.0f };
        glm::vec3 scale{ 1.0f };
        glm::quat rotation;
    };
}