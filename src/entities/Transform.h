#pragma once

#include <glm/glm.hpp>
#include "../scene/Node.h"

static const int kDirtyFlagTransform = 1 << 0;
static const int kDirtyFlagTranslation = 1 << 1;
static const int kDirtyFlagRotation = 1 << 2;
static const int kDirtyFlagScale = 1 << 3;

class OldTransform : public Node
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
	OldTransform();
	OldTransform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles);
	OldTransform(const std::string& name, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles);
	//OldTransform(const glm::mat4& transform_matrix);
	virtual glm::vec3 getLocalPosition() const;
	virtual glm::vec3 getLocalScale() const;
	virtual glm::vec3 getLocalAngles() const;
	virtual glm::vec3* getLocalPositionPointer();
	virtual glm::vec3* getLocalScalePointer();
	virtual glm::vec3* getLocalAnglesPointer();
	virtual void setLocalPosition(const glm::vec3& pos);
	virtual void setLocalScale(const glm::vec3& scale);
	virtual void setLocalAngles(const glm::vec3& angles);
	virtual glm::mat4 getTransformMatrix();
	virtual glm::mat4 getRotationMatrix();
	virtual glm::mat4 getScaleMatrix();
	virtual glm::mat4 getTranslationMatrix();
	virtual glm::vec3 getForward();
	virtual glm::vec3 getUp();
	virtual glm::vec3 getRight();
};

