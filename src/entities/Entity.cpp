#include "Entity.h"

Entity::Entity()
	: Entity(glm::vec3{ 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.0f })
{
}

Entity::Entity(const glm::vec3& pos, const glm::vec3& angles)
	: Entity(pos, glm::vec3{ 0.0f }, angles)
{
}

Entity::Entity(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& angles)
	: Transform(pos, glm::vec3(1.0f), angles), velocity(vel)
{
}

glm::vec3 Entity::getVelocity()
{
	return glm::vec3(velocity);
}

glm::vec3* Entity::getVelocityPointer()
{
	return &velocity;
}

void Entity::setVelocity(const glm::vec3& vel)
{
	this->velocity = glm::vec3{ vel };
}

