#include "Transform.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/ext/matrix_transform.hpp>

void Transform::RebuildTransformMatrix()
{
	if ((dirty_flag & kDirtyFlagRotation) != 0) {
		RebuildRotationMatrix();
	}
	transform_matrix = GetTranslationMatrix() * rotation_matrix * GetScaleMatrix();
	dirty_flag = dirty_flag & (~(kDirtyFlagTransform | kDirtyFlagTranslation | kDirtyFlagScale));
}

void Transform::RebuildRotationMatrix()
{
	float cx = glm::cos(glm::radians(angles.x));
	float cy = glm::cos(glm::radians(angles.y));
	float cz = glm::cos(glm::radians(angles.z));
	float sx = glm::sin(glm::radians(angles.x));
	float sy = glm::sin(glm::radians(angles.y));
	float sz = glm::sin(glm::radians(angles.z));
	rotation_matrix = glm::mat4{
		(cz * cy),                (sz * cy),                (-sy),           0.0f,
		(cz * sy * sx - sz * cx), (sz * sy * sx + cz * cx), (cy * sx),       0.0f,
		(cz * sy * cx + sz * sx), (sz * sy * cx - cz * sx), (cy * cx),       0.0f,
		0.0f,                     0.0f,                     0.0,             1.0f
	};
	dirty_flag = dirty_flag & (~kDirtyFlagRotation);
}

Transform::Transform()
	: position{0.0f}, scale{1.0f}, angles{0.0f}, rotation_matrix{ 1.0f }, transform_matrix{ 1.0f }
{

}

Transform::Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles)
	: position{ pos }, scale{ scale }, angles{ angles }, rotation_matrix{1.0f}, transform_matrix{1.0f}
{

}

glm::vec3 Transform::get_position()
{
	return glm::vec3{ position };
}

glm::vec3 Transform::get_scale()
{
	return glm::vec3{ scale };
}

glm::vec3 Transform::get_angles()
{
	return glm::vec3{ angles };
}

void Transform::set_position(const glm::vec3& pos)
{
	dirty_flag |= kDirtyFlagTransform | kDirtyFlagTranslation;
	this->position = glm::vec3{ pos };
}

void Transform::set_scale(const glm::vec3& scale)
{
	dirty_flag |= kDirtyFlagTransform | kDirtyFlagScale;
	this->scale = glm::vec3{ scale };
}

void Transform::set_angles(const glm::vec3& angles)
{
	dirty_flag |= kDirtyFlagTransform | kDirtyFlagRotation;
	this->angles = glm::vec3{ angles };
}

glm::mat4 Transform::GetRotationMatrix()
{
	if ((dirty_flag & kDirtyFlagRotation) != 0) {
		RebuildRotationMatrix();
	}
	return rotation_matrix;
}

glm::mat4 Transform::GetScaleMatrix()
{
	return glm::mat4{
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};
}

glm::mat4 Transform::GetTranslationMatrix()
{
	return glm::mat4{
		1.0f,       0.0f,       0.0f,       0.0f,
		0.0f,       1.0f,       0.0f,       0.0f,
		0.0f,       0.0f,       1.0f,       0.0f,
		position.x, position.y, position.z, 1.0f
	};
}

glm::vec3 Transform::GetForward()
{
	if ((dirty_flag & kDirtyFlagRotation) != 0) {
		RebuildRotationMatrix();
	}
	return glm::column(rotation_matrix, 2);
}

glm::vec3 Transform::GetUp()
{
	if ((dirty_flag & kDirtyFlagRotation) != 0) {
		RebuildRotationMatrix();
	}
	return glm::column(rotation_matrix, 1);
}

glm::vec3 Transform::GetRight()
{
	if ((dirty_flag & kDirtyFlagRotation) != 0) {
		RebuildRotationMatrix();
	}
	return glm::column(rotation_matrix, 0);
}

//Transform::Transform(const glm::mat4& transform_matrix)
//{
//}

glm::mat4 Transform::GetTransformMatrix()
{
	// by default (GLSL and GLM) spec, matrices are defined in column-order
	// so every set of four entries is one column vector
	//float cx = glm::cos(angles.x);
	//float cy = glm::cos(angles.y);
	//float cz = glm::cos(angles.z);
	//float sx = glm::sin(angles.x);
	//float sy = glm::sin(angles.y);
	//float sz = glm::sin(angles.z);
	//return glm::mat4 { 
	//    scale.x * (cz * cy),                scale.y * (sz * cy),                scale.z * (-sy),           0.0f,
	//    scale.x * (cz * sy * sx - sz * cx), scale.y * (sz * sy * sx + cz * cx), scale.z * (cy * sx),       0.0f,
	//    scale.x * (cz * sy * cx + sz * sx), scale.y * (sz * sy * cx - cz * sx), scale.z * (cy * cx),       0.0f,
	//    position.x,             position.y,             position.z,    1.0f
	//};
	if (dirty_flag) {
		RebuildTransformMatrix();
	}
	return transform_matrix;
}
