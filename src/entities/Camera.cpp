#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Camera::setPosition(const glm::vec3& pos)
{
	dirtyFlag |= kDirtyFlagView;
	Transform::setPosition(pos);
}

void Camera::setAngles(const glm::vec3& angles)
{
	dirtyFlag |= kDirtyFlagView;
	Transform::setAngles(angles);
}

float Camera::getPitch()
{
	return angles.x;
}

void Camera::setPitch(float degrees)
{
	setAngles(glm::vec3{ degrees, angles.y, angles.z });
}

float Camera::getYaw()
{
	return angles.y;
}

void Camera::setYaw(float degrees)
{
	setAngles(glm::vec3{angles.x, degrees, angles.z});
}

float Camera::getVerticalFov()
{
	return vFov;
}

void Camera::setVerticalFov(float degrees)
{
	vFov = degrees;
	dirtyFlag |= kDirtyFlagView;
}

Camera::Camera() : Camera(glm::vec3{0.0f}, glm::vec3{ 0.0f })
{
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& angles) : Transform(pos, glm::vec3{1.0f}, angles)
{
}

void Camera::RebuildViewMatrix()
{
	dirtyFlag = dirtyFlag & (~(kDirtyFlagView));
	viewMatrix = glm::lookAt(position, position - getForward(), getUp());
}

glm::mat4 Camera::GetViewMatrix()
{
	if ((dirtyFlag & kDirtyFlagView) != 0) {
		RebuildViewMatrix();
	}
	return viewMatrix;
}
