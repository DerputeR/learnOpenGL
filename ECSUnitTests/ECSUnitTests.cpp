#include "pch.h"
#include "CppUnitTest.h"
#include "../src/ecs/ECS.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace ECSUnitTests
{
	struct vec3
	{
		float x, y, z;

		bool operator ==(const vec3& other) const
		{
			return (x == other.x && y == other.y && z == other.z);
		}
	};

	struct Transform
	{
		vec3 position{ 0, 0, 0 };
		vec3 angles{ 0, 0, 0 };
		vec3 scale{ 1, 1, 1 };

		bool operator ==(const Transform& other) const
		{
			return (position == other.position && angles == other.angles && scale == other.scale);
		}
	};

	struct Rigidbody
	{
		vec3 velocity{ 0, 0, 0 };
		vec3 acceleration{ 0, 0, 0 };

		bool operator ==(const Rigidbody& other) const
		{
			return (velocity == other.velocity && acceleration == other.acceleration);
		}
	};

	TEST_CLASS(ComponentPoolTests)
	{
	public:
		TEST_METHOD(AssignEntityTest)
		{
			ECS::ComponentPool<Transform> transformPool;
			transformPool.assign(0);
			transformPool.assign(4);
			transformPool.assign(3);
			transformPool.assign(0);
			std::vector<Transform> components_expected{
				Transform{},
				Transform{},
				Transform{} 
			};

			std::vector<ECS::ComponentIndex> etc_expected(ECS::INITIAL_ENTITY_CAPACITY, ECS::INVALID_COMPONENT_INDEX);
			std::vector<ECS::EntityId> cte_expected(ECS::INITIAL_ENTITY_CAPACITY, ECS::INVALID_ENTITY_ID);

			// compare dense array
			Assert::IsTrue(components_expected.size() == transformPool.components.size());
			
			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(components_expected[i] == transformPool.components[i]);
			}

			// compare entity to component sparse set


			// compare component to entity sparse set


			// now do some operations

			// repeat the above comparisons
		}
	};
}
