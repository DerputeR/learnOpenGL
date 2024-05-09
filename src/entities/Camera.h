#pragma once
#include "Transform.h"

static const int kDirtyFlagView = 1 << 4;

class Camera : public Transform
{
protected:
	float vFov = 90.0f;
	glm::mat4 viewMatrix{ };
	void RebuildViewMatrix();
public:
	bool usePerspective = true;
	Camera();
	Camera(const glm::vec3& pos, const glm::vec3& angles);
	glm::mat4 GetViewMatrix();
	void setLocalPosition(const glm::vec3& pos) override;
	void setLocalAngles(const glm::vec3& angles) override;
	float getPitch();
	void setPitch(float degrees);
	float getYaw();
	void setYaw(float degrees);
	float getVerticalFov();
	void setVerticalFov(float degrees);
};

