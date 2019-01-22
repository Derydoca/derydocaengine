#include "EngineTestPch.h"
#include "Color.h"

TEST(Color, ColorIsOpaqueBlack_When_EmptyConstructorIsUsed) {
	DerydocaEngine::Color color;

	EXPECT_FLOAT_EQ(color.r, 0.0f);
	EXPECT_FLOAT_EQ(color.g, 0.0f);
	EXPECT_FLOAT_EQ(color.b, 0.0f);
	EXPECT_FLOAT_EQ(color.a, 1.0f);
}

TEST(Color, ValuesMatchConstructor_When_ConstructorIsCalled) {
	DerydocaEngine::Color color(0.1f, 0.2f, 0.3f, 0.4f);

	EXPECT_FLOAT_EQ(color.r, 0.1f);
	EXPECT_FLOAT_EQ(color.g, 0.2f);
	EXPECT_FLOAT_EQ(color.b, 0.3f);
	EXPECT_FLOAT_EQ(color.a, 0.4f);
}

TEST(Color, Vec3IsConvertedToColor_With_ImplicitConstructor)
{
	auto vec = glm::vec3(1.0f, 2.0f, 3.0f);
	DerydocaEngine::Color color = vec;

	EXPECT_FLOAT_EQ(vec[0], color.r);
	EXPECT_FLOAT_EQ(vec[1], color.g);
	EXPECT_FLOAT_EQ(vec[2], color.b);
	EXPECT_FLOAT_EQ(1.0f, color.a);
}

TEST(Color, Vec4IsConvertedToColor_With_ImplicitConstructor)
{
	auto vec = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
	DerydocaEngine::Color color = vec;

	EXPECT_FLOAT_EQ(vec[0], color.r);
	EXPECT_FLOAT_EQ(vec[1], color.g);
	EXPECT_FLOAT_EQ(vec[2], color.b);
	EXPECT_FLOAT_EQ(vec[3], color.a);
}
