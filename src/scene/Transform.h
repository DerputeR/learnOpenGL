#pragma once
#include "Node.h"
#include <glm/glm.hpp>

class Transform : public Node
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 angles; // todo: replace with quaternions
	glm::mat4 localMatrix;
	glm::mat4 localTransform;
	glm::mat4 localScale;
	glm::mat4 localRotation;
	//glm::mat4 globalMatrix; // todo: figure out how to cache this intelligently
	
	glm::mat4 buildLocalMatrix();
	glm::mat4 buildGlobalMatrix();
public:
	Transform();
	Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles);
	glm::vec3 getLocalPosition() const;
	glm::vec3 getLocalScale() const;
	glm::vec3 getLocalAngles() const;
	/**
	 * @brief Retrive a read-only pointer for the position vector
	 * @return Const pointer to position
	 */
	const glm::vec3* getLocalPositionPointer();
	/**
	 * @brief Retrive a read-only pointer for the scale vector
	 * @return Const pointer to scale
	 */
	const glm::vec3* getLocalScalePointer();
	/**
	 * @brief Retrive a read-only pointer for the angles vector
	 * @return Const pointer to angles
	 */
	const glm::vec3* getLocalAnglesPointer();
	virtual void setLocalPosition(const glm::vec3& pos);
	void setLocalScale(const glm::vec3& scale);
	virtual void setLocalAngles(const glm::vec3& angles);
	glm::mat4 getTransformMatrix();
	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();
};

