#include "Transform.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/ext/matrix_transform.hpp>

void Transform::rebuildTransformMatrix()
{
	if ((dirtyFlag & kDirtyFlagRotation) != 0) {
		rebuildRotationMatrix();
	}
	transformMatrix = getTranslationMatrix() * rotationMatrix * getScaleMatrix();
	dirtyFlag = dirtyFlag & (~(kDirtyFlagTransform | kDirtyFlagTranslation | kDirtyFlagScale));
}

void Transform::rebuildRotationMatrix()
{
	float cx = glm::cos(glm::radians(angles.x));
	float cy = glm::cos(glm::radians(angles.y));
	float cz = glm::cos(glm::radians(angles.z));
	float sx = glm::sin(glm::radians(angles.x));
	float sy = glm::sin(glm::radians(angles.y));
	float sz = glm::sin(glm::radians(angles.z));
	rotationMatrix = glm::mat4{
		(cz * cy),                (sz * cy),                (-sy),           0.0f,
		(cz * sy * sx - sz * cx), (sz * sy * sx + cz * cx), (cy * sx),       0.0f,
		(cz * sy * cx + sz * sx), (sz * sy * cx - cz * sx), (cy * cx),       0.0f,
		0.0f,                     0.0f,                     0.0,             1.0f
	};
	dirtyFlag = dirtyFlag & (~kDirtyFlagRotation);
}

Transform::Transform()
	: position{0.0f}, scale{1.0f}, angles{0.0f}, rotationMatrix{ 1.0f }, transformMatrix{ 1.0f }
{

}

Transform::Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& angles)
	: position{ pos }, scale{ scale }, angles{ angles }, rotationMatrix{1.0f}, transformMatrix{1.0f}
{

}

glm::vec3 Transform::getPosition()
{
	return glm::vec3{ position };
}

glm::vec3 Transform::getScale()
{
	return glm::vec3{ scale };
}

glm::vec3 Transform::getAngles()
{
	return glm::vec3{ angles };
}

glm::vec3* Transform::getPositionPointer()
{
	return &position;
}

glm::vec3* Transform::getScalePointer()
{
	return &scale;
}

glm::vec3* Transform::getAnglesPointer()
{
	return &angles;
}

void Transform::setPosition(const glm::vec3& pos)
{
	dirtyFlag |= kDirtyFlagTransform | kDirtyFlagTranslation;
	this->position = glm::vec3{ pos };
}

void Transform::setScale(const glm::vec3& scale)
{
	dirtyFlag |= kDirtyFlagTransform | kDirtyFlagScale;
	this->scale = glm::vec3{ scale };
}

void Transform::setAngles(const glm::vec3& angles)
{
	dirtyFlag |= kDirtyFlagTransform | kDirtyFlagRotation;
	this->angles = glm::vec3{ angles };
}

glm::mat4 Transform::getRotationMatrix()
{
	if ((dirtyFlag & kDirtyFlagRotation) != 0) {
		rebuildRotationMatrix();
	}
	return rotationMatrix;
}

glm::mat4 Transform::getScaleMatrix()
{
	return glm::mat4{
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};
}

glm::mat4 Transform::getTranslationMatrix()
{
	return glm::mat4{
		1.0f,       0.0f,       0.0f,       0.0f,
		0.0f,       1.0f,       0.0f,       0.0f,
		0.0f,       0.0f,       1.0f,       0.0f,
		position.x, position.y, position.z, 1.0f
	};
}

glm::vec3 Transform::getForward()
{
	if ((dirtyFlag & kDirtyFlagRotation) != 0) {
		rebuildRotationMatrix();
	}
	return glm::column(rotationMatrix, 2);
}

glm::vec3 Transform::getUp()
{
	if ((dirtyFlag & kDirtyFlagRotation) != 0) {
		rebuildRotationMatrix();
	}
	return glm::column(rotationMatrix, 1);
}

glm::vec3 Transform::getRight()
{
	if ((dirtyFlag & kDirtyFlagRotation) != 0) {
		rebuildRotationMatrix();
	}
	return glm::column(rotationMatrix, 0);
}

//Transform::Transform(const glm::mat4& transform_matrix)
//{
//}

glm::mat4 Transform::getTransformMatrix()
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
	if (dirtyFlag) {
		rebuildTransformMatrix();
	}
	return transformMatrix;
}
