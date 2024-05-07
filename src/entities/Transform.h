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
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getScale() const;
	virtual glm::vec3 getAngles() const;
	virtual glm::vec3* getPositionPointer();
	virtual glm::vec3* getScalePointer();
	virtual glm::vec3* getAnglesPointer();
	virtual void setPosition(const glm::vec3& pos);
	virtual void setScale(const glm::vec3& scale);
	virtual void setAngles(const glm::vec3& angles);
	virtual glm::mat4 getTransformMatrix();
	virtual glm::mat4 getRotationMatrix();
	virtual glm::mat4 getScaleMatrix();
	virtual glm::mat4 getTranslationMatrix();
	virtual glm::vec3 getForward();
	virtual glm::vec3 getUp();
	virtual glm::vec3 getRight();
};

