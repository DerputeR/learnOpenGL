#include "pch.h"
#include "CppUnitTest.h"
#include "../src/ecs/ECS.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace ECSUnitTests
{
	struct vec3
	{
		float x, y, z;
	};

	struct Transform
	{
		vec3 position{ 0, 0, 0 };
		vec3 angles{ 0, 0, 0 };
		vec3 scale{ 1, 1, 1 };
	};

	struct Rigidbody
	{
		vec3 velocity{ 0, 0, 0 };
		vec3 acceleration{ 0, 0, 0 };
	};

	TEST_CLASS(ECSUnitTests)
	{
	public:
		TEST_METHOD(ComponentPoolTests)
		{
			ECS::ComponentPool<Transform> transformPool;
			transformPool.assign(0);
		}
	};
}
