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

Camera::Camera() : Camera(glm::vec3{0.0f}, glm::vec3{ 0.0f })
{
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& angles) : Transform(pos, glm::vec3{1.0f}, angles)
{
}

void Camera::RebuildViewMatrix()
{
	dirtyFlag = dirtyFlag & (~(kDirtyFlagView));
	view_matrix = glm::lookAt(position, position - getForward(), getUp());
}

glm::mat4 Camera::GetViewMatrix()
{
	if ((dirtyFlag & kDirtyFlagView) != 0) {
		RebuildViewMatrix();
	}
	return view_matrix;
}
