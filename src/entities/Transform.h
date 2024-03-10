#pragma once

#include <glm/glm.hpp>

static const int kDirtyFlagTransform = 1 << 0;
static const int kDirtyFlagTranslation = 1 << 1;
static const int kDirtyFlagRotation = 1 << 2;
static const int kDirtyFlagScale = 1 << 3;

class Transform
{
protected:
	int dirtyFlag = ~0;
	glm::mat4 rotationMatrix;
	glm::mat4 transformMatrix;
	void rebuildTransformMatrix();
	void rebuildRotationMatrix();
	glm::vec3 position;
	glm::vec3 scale;
	// todo: replace with quaternions
	glm::vec3 angles;
public:
	Transform();
	Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles);
	//Transform(const glm::mat4& transform_matrix);
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getAngles();
	glm::vec3* getPositionPointer();
	glm::vec3* getScalePointer();
	glm::vec3* getAnglesPointer();
	virtual void setPosition(const glm::vec3& pos);
	virtual void setScale(const glm::vec3& scale);
	virtual void setAngles(const glm::vec3& angles);
	glm::mat4 getTransformMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getTranslationMatrix();
	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();
};

