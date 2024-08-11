#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Camera::setLocalPosition(const glm::vec3& pos)
{
	//dirtyFlag |= kDirtyFlagView;
	OldTransform::setLocalPosition(pos);
	RebuildViewMatrix();
}

void Camera::setLocalAngles(const glm::vec3& angles)
{
	//dirtyFlag |= kDirtyFlagView;
	OldTransform::setLocalAngles(angles);
	RebuildViewMatrix();
}

float Camera::getPitch()
{
	return getLocalAngles().x;
}

void Camera::setPitch(float degrees)
{
	setLocalAngles(glm::vec3{ degrees, getLocalAngles().y, getLocalAngles().z});
}

float Camera::getYaw()
{
	return getLocalAngles().y;
}

void Camera::setYaw(float degrees)
{
	setLocalAngles(glm::vec3{getLocalAngles().x, degrees, getLocalAngles().z});
}

float Camera::getVerticalFov()
{
	return vFov;
}

void Camera::setVerticalFov(float degrees)
{
	vFov = degrees;
	RebuildViewMatrix();
	//dirtyFlag |= kDirtyFlagView;
}

Camera::Camera() : Camera(glm::vec3{0.0f}, glm::vec3{ 0.0f })
{
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& angles) : OldTransform(pos, glm::vec3{1.0f}, angles)
{
}

void Camera::RebuildViewMatrix()
{
	//dirtyFlag = dirtyFlag & (~(kDirtyFlagView));
	viewMatrix = glm::lookAt(getLocalPosition(), getLocalPosition() - getForward(), getUp());
}

glm::mat4 Camera::GetViewMatrix()
{
	//if ((dirtyFlag & kDirtyFlagView) != 0) {
		//RebuildViewMatrix();
	//}
	return viewMatrix;
}
