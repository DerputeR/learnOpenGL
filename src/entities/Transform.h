#pragma once

#include <glm/glm.hpp>

static const int kDirtyFlagTransform = 1 << 0;
static const int kDirtyFlagTranslation = 1 << 1;
static const int kDirtyFlagRotation = 1 << 2;
static const int kDirtyFlagScale = 1 << 3;

class Transform
{
protected:
	int dirty_flag = ~0;
	glm::mat4 rotation_matrix;
	glm::mat4 transform_matrix;
	void RebuildTransformMatrix();
	void RebuildRotationMatrix();
	glm::vec3 position;
	glm::vec3 scale;
	// todo: replace with quaternions
	glm::vec3 angles;
public:
	Transform();
	Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles);
	//Transform(const glm::mat4& transform_matrix);
	glm::vec3 get_position();
	glm::vec3 get_scale();
	glm::vec3 get_angles();
	virtual void set_position(const glm::vec3& pos);
	virtual void set_scale(const glm::vec3& scale);
	virtual void set_angles(const glm::vec3& angles);
	glm::mat4 GetTransformMatrix();
	glm::mat4 GetRotationMatrix();
	glm::mat4 GetScaleMatrix();
	glm::mat4 GetTranslationMatrix();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
};

