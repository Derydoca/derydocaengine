#include "EngineTestPch.h"
#include "GameObject.h"
#include <memory>

TEST(GameObject, ParentOfChildIsSet_When_ChildIsAdded) {
	auto gameObjectParent = std::make_shared<DerydocaEngine::GameObject>("Parent");
	auto gameObjectChild =  std::make_shared<DerydocaEngine::GameObject>("Child");

	gameObjectParent->addChild(gameObjectChild);

	EXPECT_EQ(gameObjectChild->getParent(), gameObjectParent);
}

TEST(GameObject, NameIsChanged_When_SetNameIsCalled) {
	auto gameObject = std::make_shared<DerydocaEngine::GameObject>("Old Name");

	std::string newName = "New name";
	gameObject->setName(newName);

	EXPECT_EQ(newName, gameObject->getName());
}