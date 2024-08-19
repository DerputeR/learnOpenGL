#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ECS::Components
{

    bool cascadeDirtyFlag(Entity parent)
    {

    }

    void ParentedComponent::setParent(Entity parent)
    {
        if (!owningScene->isAlive(parent)) return;
        Entity lastParent = parent;
        this->parent = parent;
        Transform* t = owningScene->getComponent<Transform>(owningEntity);
        if (t)
        {
            t->setDirty();
            // cascade this down to all children of t
            cascadeDirtyFlag(owningEntity);
        }
    }

    Entity ParentedComponent::getParent()
    {
        return parent;
    }

    void Transform::setDirty()
    {
        this->dirty = true;
    }

    glm::vec3 Transform::getLocalPosition() const
    {
        return position;
    }

    glm::quat Transform::getLocalRotation() const
    {
        return rotation;
    }

    glm::vec3 Transform::getLocalScale() const
    {
        return scale;
    }

    glm::vec3 Transform::getPosition()
    {
        // return glm::vec3{ parent->getMatrix() * glm::vec4{ position, 1.0f } };
        return glm::vec3{};
    }

    glm::quat Transform::getRotation()
    {
        // return glm::quat_cast(parent->getMatrix() * glm::mat4_cast(this->rotation));
        return glm::quat{};
    }

    glm::mat4 Transform::getMatrix()
    {
        if (dirty)
        {
            // translation * rotation * scale
            // TODO: 
            glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, this->position);
            glm::mat4 rotation = glm::mat4_cast(this->rotation);
            glm::mat4 scale = glm::scale(glm::mat4{ 1.0f }, this->scale);
            transformationMatrix = translation * rotation * scale;
        }
        return transformationMatrix;
    }

    void Transform::setLocalPosition(const glm::vec3& position)
    {
        dirty = true;
        this->position = position;
    }

    void Transform::setLocalScale(const glm::vec3& scale)
    {
        dirty = true;
        this->scale = scale;
    }

    void Transform::setLocalRotation(const glm::quat& rotation)
    {
        dirty = true;
        this->rotation = rotation;
    }

    void Transform::setLocalRotation(float angle, const glm::vec3& axis)
    {
        dirty = true;
        this->rotation = glm::angleAxis(angle, axis);
    }

    void Transform::setLocalRotation(float pitch, float yaw, float roll)
    {
        dirty = true;
        this->rotation = glm::quat(glm::vec3{ pitch, yaw, roll });
    }
}