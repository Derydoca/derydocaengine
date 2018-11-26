#include "pch.h"
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