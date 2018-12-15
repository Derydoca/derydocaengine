#include "EngineTestPch.h"
#include "Components\Transform.h"

TEST(Transform, ModelIsIdentity_When_ConstructedWithNoParameters) {
	DerydocaEngine::Components::Transform transform;

	auto identityMatrix = glm::mat4();
	auto transModel = transform.getModel();

	EXPECT_EQ(transModel, identityMatrix);
}

TEST(Transform, ModelHasTransform_When_PositionIsSet)
{
	DerydocaEngine::Components::Transform transform;

	glm::vec3 newPos(1.1f, 2.2f, 3.3f);
	transform.setPos(newPos);

	auto transModel = transform.getModel();

	EXPECT_FLOAT_EQ(transModel[3].x, newPos.x);
	EXPECT_FLOAT_EQ(transModel[3].y, newPos.y);
	EXPECT_FLOAT_EQ(transModel[3].z, newPos.z);
}

TEST(Transform, ModelHasScale_When_ScaleIsSet)
{
	DerydocaEngine::Components::Transform transform;

	glm::vec3 newScale(1.1f, 2.2f, 3.3f);
	transform.setScale(newScale);

	auto transModel = transform.getModel();

	EXPECT_FLOAT_EQ(transModel[0].x, newScale.x);
	EXPECT_FLOAT_EQ(transModel[1].y, newScale.y);
	EXPECT_FLOAT_EQ(transModel[2].z, newScale.z);
}