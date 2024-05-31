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
				Transform{{1, 0, 0}, {0, 100, 0}, {1, 1, 1}},
				Transform{{0, 1, 0}, {90, 0, 0}, {1, 1, 1}},
				Transform{{0, 0, 1}, {0, 10, 10}, {1, 1, 1}}
			};
			transformPool.components[0].position = {1, 0, 0};
			transformPool.components[0].angles = { 0, 100, 0 };
			transformPool.components[0].scale = { 1, 1, 1 };

			transformPool.components[1].position = { 0, 1, 0 };
			transformPool.components[1].angles = { 90, 0, 0 };
			transformPool.components[1].scale = { 1, 1, 1 };

			transformPool.components[2].position = { 0, 0, 1 };
			transformPool.components[2].angles = { 0, 10, 10 };
			transformPool.components[2].scale = { 1, 1, 1 };


			std::vector<ECS::component_index> etc_expected(ECS::INITIAL_ENTITY_CAPACITY, ECS::INVALID_COMPONENT_INDEX);
			etc_expected[0] = 0;
			etc_expected[4] = 1;
			etc_expected[3] = 2;
			std::vector<ECS::entity_id> cte_expected(ECS::INITIAL_ENTITY_CAPACITY, ECS::INVALID_ENTITY_ID);
			cte_expected[0] = 0;
			cte_expected[1] = 4;
			cte_expected[2] = 3;

			// compare dense array
			Assert::IsTrue(components_expected.size() == transformPool.components.size());
			
			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(components_expected[i] == transformPool.components[i]);
			}

			// compare entity to component sparse set
			Assert::IsTrue(etc_expected.size() == transformPool.componentMap.sparseMap.size());

			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(etc_expected[i] == transformPool.componentMap.sparseMap[i]);
			}


			// compare component to entity packed set
			Assert::IsTrue(cte_expected.size() == transformPool.componentMap.packedMap.size());

			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(cte_expected[i] == transformPool.componentMap.packedMap[i]);
			}

			// now removeComponent an entity
			components_expected[1] = components_expected[2];
			components_expected.pop_back();
			transformPool.unassign(4);
			etc_expected[0] = 0;
			etc_expected[4] = ECS::INVALID_COMPONENT_INDEX;
			etc_expected[3] = 1;
			cte_expected[0] = 0;
			cte_expected[1] = 3;
			cte_expected[2] = ECS::INVALID_ENTITY_ID;

			Assert::IsTrue(etc_expected.size() == transformPool.componentMap.sparseMap.size());
			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(etc_expected[i] == transformPool.componentMap.sparseMap[i]);
			}

			Assert::IsTrue(cte_expected.size() == transformPool.componentMap.packedMap.size());
			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(cte_expected[i] == transformPool.componentMap.packedMap[i]);
			}
		}
	};

	TEST_CLASS(SceneTests)
	{		
		ECS::Scene scene;
		ECS::Entity player;
		ECS::Entity camera;
		ECS::Entity physObj;

	public:
		TEST_METHOD_INITIALIZE(ECSInit)
		{
			scene = ECS::Scene{};
			player = scene.createEntity();
			camera = scene.createEntity();
			physObj = scene.createEntity();
		}

		TEST_METHOD(SingleComponentTest)
		{
			Transform* t = scene.getComponent<Transform>(player);
			Assert::IsTrue(t == nullptr);

			scene.addComponent<Transform>(player);
			t = scene.getComponent<Transform>(player);
			Assert::IsTrue(t != nullptr);

			t->position.x = 5;
			Assert::IsTrue(scene.getComponent<Transform>(player)->position.x == 5);

			scene.addComponent<Transform>(player);
			Assert::IsTrue(scene.getComponent<Transform>(player)->position.x == 5);

			scene.removeComponent<Transform>(player);
			t = scene.getComponent<Transform>(player);
			Assert::IsTrue(t == nullptr);
		}

		//TEST_METHOD(EntitiesListTest)
		//{
		//	auto e = scene.getEntities();
		//	e.push_back(ECS::Entity{ 12, 12 });
		//	auto e2 = scene.getEntities();
		//	Assert::IsTrue(e.size() != e2.size()); // make sure read-only is working

		//	ECS::Entity ant = scene.createEntity();
		//	auto& e3 = scene.getEntities();
		//	Assert::IsTrue(e2 != e3);
		//	bool isUpdated = false;
		//	for (const auto& f : e3)
		//	{
		//		if (f == ant)
		//		{
		//			isUpdated = true;
		//		}
		//	}
		//	Assert::IsTrue(isUpdated);
		//}
	};
}
