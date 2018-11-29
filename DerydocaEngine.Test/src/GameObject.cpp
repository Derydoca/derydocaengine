#include "pch.h"
#include "GameObject.h"
#include <memory>

TEST(GameObject, ParentOfChildIsSet_When_ChildIsAdded) {
	auto gameObjectParent = DerydocaEngine::GameObject::generate("Parent");
	auto gameObjectChild = DerydocaEngine::GameObject::generate("Child");

	gameObjectParent->addChild(gameObjectChild);

	EXPECT_EQ(gameObjectChild->getParent(), gameObjectParent);
}
