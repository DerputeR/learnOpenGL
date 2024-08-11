#include "pch.h"
#include "CppUnitTest.h"
#include "../src/ecs/ECS.h"
#include <vector>
#include <iostream>

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
			std::vector<ECS::entity_id> cte_expected{ 0, 4, 3 };

			// compare dense array
			Assert::IsTrue(components_expected.size() == transformPool.components.size());
			
			for (size_t i = 0; i < components_expected.size(); i++)
			{
				Assert::IsTrue(components_expected[i] == transformPool.components[i]);
			}

			// compare entity to component sparse set
			Assert::IsTrue(etc_expected.size() == transformPool.componentMap.getSparse().size());
			for (ECS::entity_id i = 0; i < static_cast<ECS::entity_id>(components_expected.size()); i++)
			{
				Assert::IsTrue(etc_expected[i] == transformPool.componentMap[i]);
			}

			// compare component to entity packed set
			Assert::IsTrue(cte_expected.size() == transformPool.componentMap.getPacked().size());
			for (ECS::component_index i = 0; i < static_cast<ECS::component_index>(components_expected.size()); i++)
			{
				Assert::IsTrue(cte_expected[i] == transformPool.componentMap(i));
			}

			// now remove an entity
			components_expected[1] = components_expected[2];
			components_expected.pop_back();
			transformPool.unassign(4);
			etc_expected[0] = 0;
			etc_expected[4] = ECS::INVALID_COMPONENT_INDEX;
			etc_expected[3] = 1;
			cte_expected[0] = 0;
			cte_expected[1] = 3;
			cte_expected.pop_back();

			// compare entity to component sparse set
			Assert::IsTrue(etc_expected.size() == transformPool.componentMap.getSparse().size());
			for (ECS::entity_id i = 0; i < static_cast<ECS::entity_id>(components_expected.size()); i++)
			{
				Assert::IsTrue(etc_expected[i] == transformPool.componentMap[i]);
			}

			// compare component to entity packed set
			Assert::IsTrue(cte_expected.size() == transformPool.componentMap.getPacked().size());
			for (ECS::component_index i = 0; i < static_cast<ECS::component_index>(components_expected.size()); i++)
			{
				Assert::IsTrue(cte_expected[i] == transformPool.componentMap(i));
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
			scene = ECS::Scene();
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

		TEST_METHOD(EntitiesListTest)
		{
			auto* e = scene.getEntities();
			//e->push_back(ECS::Entity{ 12, 12 }); // illegal op

			ECS::Entity ant = scene.createEntity();
			auto* e2 = scene.getEntities();
			Assert::IsTrue(e == e2); // pointer is to a live list and should reflect this
			bool isUpdated = false;
			for (const auto& f : *e2)
			{
				if (f == ant)
				{
					isUpdated = true;
				}
			}
			Assert::IsTrue(isUpdated);
		}

		TEST_METHOD(EntityDestroyTest)
		{
			scene.addComponent<Transform>(player);
			scene.addComponent<Rigidbody>(player);
			scene.addComponent<Transform>(camera);
			scene.addComponent<Transform>(physObj);
			scene.addComponent<Rigidbody>(physObj);

			scene.getComponent<Transform>(player)->position.x = 10;
			scene.getComponent<Transform>(physObj)->position.x = 5;

			auto* transformPool = scene.getComponentPool<Transform>();
			Assert::IsTrue(transformPool->size() == 3);
			Assert::IsTrue((*transformPool)[0].position.x == 10);

			scene.destroyEntity(player);
			Assert::IsTrue(transformPool->size() == 2);
			Assert::IsTrue((*transformPool)[0].position.x == 5);

			player = scene.createEntity();
			Assert::IsTrue(transformPool->size() == 2);
			Assert::IsTrue((*transformPool)[0].position.x == 5);

			scene.addComponent<Transform>(player);
			Assert::IsTrue(transformPool->size() == 3);
			Assert::IsTrue((*transformPool)[0].position.x == 5);
			Assert::IsTrue((*transformPool)[2].position.x == 0);

			scene.getComponent<Transform>(player)->position.x = -10;
			Assert::IsTrue((*transformPool)[2].position.x == -10);
		}
	};

	TEST_CLASS(SceneViewTests)
	{
		ECS::Scene scene;
		ECS::Entity player;
		ECS::Entity camera;
		ECS::Entity physObj;
		ECS::Entity dummy;

	public:
		TEST_METHOD_INITIALIZE(ECSInit)
		{
			scene = ECS::Scene();
			player = scene.createEntity();
			camera = scene.createEntity();
			physObj = scene.createEntity();
			dummy = scene.createEntity();

			scene.addComponent<Transform>(player);
			scene.addComponent<Transform>(camera);
			scene.getComponent<Transform>(camera)->position.y = 1.5f;

			scene.addComponent<Transform>(physObj);
			scene.getComponent<Transform>(physObj)->angles = { 45.0f, 45.0f, 45.0f };
			scene.addComponent<Rigidbody>(physObj);
			scene.getComponent<Rigidbody>(physObj)->acceleration.y = -9.81f;

			scene.addComponent<Rigidbody>(dummy);

		}

		TEST_METHOD(AllComponentsView)
		{
			ECS::SceneView<> entities{ scene };
			std::vector<ECS::Entity> listedEntities;
			std::vector<ECS::Entity> expectedEntities { player, camera, physObj, dummy };
			for (auto& entity : entities)
			{
				listedEntities.push_back(entity);
			}

			for (int i = 0; i < listedEntities.size(); i++)
			{
				Assert::IsTrue(listedEntities[i] == expectedEntities[i]);
			}
		}

		TEST_METHOD(SingleComponentView_Transform)
		{
			ECS::SceneView<Transform> entities{ scene };
			std::vector<ECS::Entity> listedEntities;
			std::vector<ECS::Entity> expectedEntities{ player, camera, physObj };
			for (auto& entity : entities)
			{
				listedEntities.push_back(entity);
			}

			for (int i = 0; i < listedEntities.size(); i++)
			{
				Assert::IsTrue(listedEntities[i] == expectedEntities[i]);
			}
		}

		TEST_METHOD(SingleComponentView_Rigidbody)
		{
			ECS::SceneView<Rigidbody> entities{ scene };
			std::vector<ECS::Entity> listedEntities;
			std::vector<ECS::Entity> expectedEntities{ physObj, dummy };
			for (auto& entity : entities)
			{
				listedEntities.push_back(entity);
			}

			for (int i = 0; i < listedEntities.size(); i++)
			{
				Assert::IsTrue(listedEntities[i] == expectedEntities[i]);
			}
		}

		TEST_METHOD(MultiComponentView_TransformAndRigidbody)
		{
			ECS::SceneView<Transform, Rigidbody> entities{ scene };
			std::vector<ECS::Entity> listedEntities;
			std::vector<ECS::Entity> expectedEntities{ physObj };
			for (auto& entity : entities)
			{
				listedEntities.push_back(entity);
			}

			for (int i = 0; i < listedEntities.size(); i++)
			{
				Assert::IsTrue(listedEntities[i] == expectedEntities[i]);
			}
		}
	};
}
