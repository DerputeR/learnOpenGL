#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace ECS::Components
{
    struct Transform
    {
        using transform_id = unsigned int;
        const static transform_id NULL_TRANSFORM = -1;

        transform_id parent_id;
        // local transforms
        glm::vec3 local_position{ 0.0f };
        glm::vec3 local_scale{ 1.0f };
        glm::quat local_rotation;
        // global transforms
        glm::mat4 global_transforms{ 1.0f };
        bool dirty = false;
    };

    class TransformHierarchy
    {
        std::vector<Transform> transformPool;
    };
}