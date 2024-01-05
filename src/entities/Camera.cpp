#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Camera::set_position(const glm::vec3& pos)
{
	dirty_flag |= kDirtyFlagView;
	Transform::set_position(pos);
}

void Camera::set_angles(const glm::vec3& angles)
{
	dirty_flag |= kDirtyFlagView;
	Transform::set_angles(angles);
}

Camera::Camera() : Camera(glm::vec3{0.0f}, glm::vec3{ 0.0f })
{
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& angles) : Transform(pos, glm::vec3{1.0f}, angles)
{
}

void Camera::RebuildViewMatrix()
{
	dirty_flag = dirty_flag & (~(kDirtyFlagView));
	view_matrix = glm::lookAt(position, position - GetForward(), GetUp());
}

glm::mat4 Camera::GetViewMatrix()
{
	if ((dirty_flag & kDirtyFlagView) != 0) {
		RebuildViewMatrix();
	}
	return view_matrix;
}
