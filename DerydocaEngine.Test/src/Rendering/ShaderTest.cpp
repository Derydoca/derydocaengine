#include "EngineTestPch.h"
#include "Rendering\Shader.h"

TEST(Shader, Load)
{
	DerydocaEngine::Rendering::Shader("..\\exampleProject\\shaders\\vertShading.hlsl");
	EXPECT_EQ(true, true);
}