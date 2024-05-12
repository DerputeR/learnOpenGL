#include "Transform.h"
#include <glm/gtc/matrix_access.hpp>

static glm::mat4 buildRotationMatrix(const glm::vec3& angles);
static glm::mat4 buildTranslationMatrix(const glm::vec3& position);
static glm::mat4 buildScaleMatrix(const glm::vec3& scale);

glm::mat4 Transform::buildLocalMatrix()
{
	glm::mat4 rotationMatrix = buildRotationMatrix(angles);
	glm::mat4 translationMatrix = buildTranslationMatrix(position);
	glm::mat4 scaleMatrix = buildScaleMatrix(scale);
	glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return transformMatrix;
}

glm::mat4 Transform::buildGlobalMatrix()
{
	// todo: probably turn transform into a component attached to nodes instead
	// of inheriting node...
	glm::mat4 matrix = localMatrix;
	Node* parent = this->getParent();
	while (parent != nullptr)
	{
		Transform* parentTransform = dynamic_cast<Transform*>(parent);
		if (parentTransform)
		{
			matrix = parentTransform->localMatrix * matrix;
		}
		parent = parent->getParent();
		// continue going up the tree, maybe we'll encounter another transform...
	}
	// todo: somehow include a dirty flag system that only calls this as needed
	return matrix;
}

static glm::mat4 buildRotationMatrix(const glm::vec3& angles)
{
	float cx = glm::cos(glm::radians(angles.x));
	float cy = glm::cos(glm::radians(angles.y));
	float cz = glm::cos(glm::radians(angles.z));
	float sx = glm::sin(glm::radians(angles.x));
	float sy = glm::sin(glm::radians(angles.y));
	float sz = glm::sin(glm::radians(angles.z));
	glm::mat4 rotationMatrix = glm::mat4{
		(cz * cy),                (sz * cy),                (-sy),           0.0f,
		(cz * sy * sx - sz * cx), (sz * sy * sx + cz * cx), (cy * sx),       0.0f,
		(cz * sy * cx + sz * sx), (sz * sy * cx - cz * sx), (cy * cx),       0.0f,
		0.0f,                     0.0f,                     0.0,             1.0f
	};
	return rotationMatrix;
}

static glm::mat4 buildTranslationMatrix(const glm::vec3& position)
{
	return glm::mat4
	{
		1.0f,       0.0f,       0.0f,       0.0f,
		0.0f,       1.0f,       0.0f,       0.0f,
		0.0f,       0.0f,       1.0f,       0.0f,
		position.x, position.y, position.z, 1.0f
	};
}

static glm::mat4 buildScaleMatrix(const glm::vec3& scale)
{
	return glm::mat4 {
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};
}

Transform::Transform() :
	position{ 0.0f },
	scale{ 1.0f },
	angles{ 0.0f },
	Node{ "OldTransform" }
{ }

Transform::Transform(
	const glm::vec3& pos,
	const glm::vec3& scale,
	const glm::vec3& angles) :
	position{ pos },
	scale{ scale },
	angles{ angles },
	Node{ "OldTransform" }
{ }

glm::vec3 Transform::getLocalPosition() const
{
	return glm::vec3{ position };
}

glm::vec3 Transform::getLocalScale() const
{
	return glm::vec3{ scale };
}

glm::vec3 Transform::getLocalAngles() const
{
	return glm::vec3{ angles };
}

const glm::vec3* Transform::getLocalPositionPointer()
{
	return &position;
}

const glm::vec3* Transform::getLocalScalePointer()
{
	return &scale;
}

const glm::vec3* Transform::getLocalAnglesPointer()
{
	return &angles;
}

void Transform::setLocalPosition(const glm::vec3& pos)
{
	this->position = glm::vec3{ pos };
	localMatrix = buildLocalMatrix();
}

void Transform::setLocalScale(const glm::vec3& scale)
{
	this->scale = glm::vec3{ scale };
	localMatrix = buildLocalMatrix();
}

void Transform::setLocalAngles(const glm::vec3& angles)
{
	this->angles = glm::vec3{ angles };
	localMatrix = buildLocalMatrix();
}

glm::mat4 Transform::getTransformMatrix()
{
	return buildGlobalMatrix();
}

glm::vec3 Transform::getForward()
{
	return glm::column(localRotation, 2);
}

glm::vec3 Transform::getUp()
{
	return glm::column(localRotation, 1);
}

glm::vec3 Transform::getRight()
{
	return glm::column(localRotation, 0);
}