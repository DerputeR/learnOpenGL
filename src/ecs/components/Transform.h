#pragma once
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

struct Transform
{
    Transform* parent;
    // global transforms
    glm::vec3 position{ 0.0f };
    glm::vec3 scale{ 1.0f };
    glm::quat rotation;
};