#pragma once
#include "Transform.h"

static const int kDirtyFlagView = 1 << 4;

class Camera : public Transform
{
protected:
	glm::mat4 view_matrix{ };
	void RebuildViewMatrix();
public:
	Camera();
	Camera(const glm::vec3& pos, const glm::vec3& angles);
	glm::mat4 GetViewMatrix();
	void setPosition(const glm::vec3& pos) override;
	void setAngles(const glm::vec3& angles) override;
};

