#include "pch.h"
#include "CppUnitTest.h"
#include "../src/ecs/ECS.h"
#include "../src/ecs/components/Transform.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TransformUnitTests
{
	using namespace ECS::Components;

	TEST_CLASS(TransformTests)
	{
		ECS::Scene registry;
		ECS::Entity obj;
		ECS::Entity childObj;

	public:
		TEST_METHOD_INITIALIZE(ECSInit)
		{
			registry = ECS::Scene();
			obj = registry.createEntity();
			childObj = registry.createEntity();
			registry.addComponent<Transform>(obj);
			registry.addComponent<Transform>(childObj);
			Transform* parentTransform = registry.getComponent<Transform>(obj);
			Transform* childTransform = registry.getComponent<Transform>(childObj);
			
		}

		TEST_METHOD(TranslationTest)
		{
			Transform* transform = registry.getComponent<Transform>(obj);
			
		}

		TEST_METHOD(RotationTest)
		{
			Transform* transform = registry.getComponent<Transform>(obj);
			
		}

		TEST_METHOD(ScaleTest)
		{
			Transform* transform = registry.getComponent<Transform>(obj);
			
		}
	};
}
