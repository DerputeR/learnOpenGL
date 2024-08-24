#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include "../ECS.h"

namespace ECS::Components
{
    class ParentedComponent
    {
    public:
        /**
         * @brief Sets the parent entity. If the child entity has a Transform component, its dirty flag is set.
         * @param parent
         */
        void setParent(Entity parent);

        /**
         * @brief Returns the parent Entity
         * @return The parent entity
         */
        Entity getParent();
    private:
        Entity parent = ECS::INVALID_ENTITY;
    };

    class Transform
    {
    public:
        /**
         * @return A copy of this Transform's local position vector
         */
        glm::vec3 getLocalPosition() const;

        /**
         * @return A copy of this Transform's local rotation quaternion
         */
        glm::quat getLocalRotation() const;

        /**
         * @return A copy of this Transform's local scale vector
         */
        glm::vec3 getLocalScale() const;

        /**
         * @brief Copies the local position with the parent's transformation matrix pre-multiplied to it, if one exists.
         * @return A copy of this Transform's global position vector
         */
        glm::vec3 getPosition();

        /**
         * @brief Copies the local rotation with the parent's transformation matrix pre-multiplied to it, if one exists.
         * @return A copy of this Transform's global rotation quaternion
         */
        glm::quat getRotation();

        /**
         * @brief Combines this Transform's position, rotation, and scale into one 4x4 matrix.
         * @return A copy of this Transform's transformation matrix
         */
        glm::mat4 getMatrix();

        /// TODO: NEED TO PROPOGATE DIRTY FLAG SET TO CHILDREN

        /**
         * @brief Updates this Transform's local position vector
         * @param position 
         */
        void setLocalPosition(const glm::vec3& position);

        /**
         * @brief Updates this Transform's local scale vector
         * @param scale
         */
        void setLocalScale(const glm::vec3& scale);

        /**
         * @brief Updates this Transform's local rotation quaternion
         * @param rotation 
         */
        void setLocalRotation(const glm::quat& rotation);

        /**
         * @brief Updates this Transform's local rotation quaternion using angle-axis
         * @param angle 
         * @param axis 
         */
        void setLocalRotation(float angle, const glm::vec3& axis);

        /**
         * @brief Updates this Transform's local rotation quaternion using pitch, yaw, roll (XYZ Euler)
         * @param pitch 
         * @param yaw 
         * @param roll 
         */
        void setLocalRotation(float pitch, float yaw, float roll);

        /**
         * @brief Manually set the dirty flag to true
         */
        void setDirty();
        // TODO: methods for setting global position and rotation. scale is kinda tricky...


    private:

        Entity parent = ECS::INVALID_ENTITY;

        // local transforms
        
        glm::vec3 position{ 0.0f };
        glm::vec3 scale{ 1.0f };
        glm::quat rotation{ };

        // global transforms

        glm::mat4 transformationMatrix{ 1.0f };
        bool dirty = true;
    };
}