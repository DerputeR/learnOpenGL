#pragma once
#include "Transform.h"

/**
 * @brief Entities are transforms that are meant to dynamically move
 */
class Entity : public OldTransform
{
protected:
	glm::vec3 velocity;
public:
	Entity();
	Entity(const glm::vec3& pos, const glm::vec3& angles);
	Entity(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& angles);
	virtual glm::vec3 getVelocity();
	virtual glm::vec3* getVelocityPointer();
	virtual void setVelocity(const glm::vec3& vel);
};

