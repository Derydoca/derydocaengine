#include "EngineTestPch.h"
#include "Rendering\Material.h"
#include "Color.h"
#include "Rendering\Texture.h"

//TEST(Material, MaterialIsCopiedToOtherMaterial_When_CopyFromFunctionIsCalled) {
//	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();
//	
//	auto newMaterial = std::make_shared<DerydocaEngine::Rendering::Material>();
//	newMaterial = std::make_shared<DerydocaEngine::Rendering::Material>();
//	newMaterial->copyFrom(material);
//
//	std::string paramName = "MyInt";
//
//	EXPECT_NE(material, newMaterial);
//	EXPECT_EQ(material->getInt(paramName), newMaterial->getInt(paramName));
//}

//TEST(Material, IntReturned_After_BeingSet)
//{
//	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();
//
//	std::string propertyName = "MyProperty";
//
//	material->setInt(propertyName, 25);
//	EXPECT_TRUE(material->intExists(propertyName));
//}

TEST(Material, BoolExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setBool(propertyName, true);
	EXPECT_TRUE(material->boolExists(propertyName));
}

TEST(Material, ColorRGBExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setColorRGB(propertyName, DerydocaEngine::Color(1.0f, 0.0f, 0.0f));
	EXPECT_TRUE(material->colorRGBExists(propertyName));
}

TEST(Material, ColorRGBAExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setColorRGBA(propertyName, DerydocaEngine::Color(1.0f, 0.0f, 0.0f, 1.0f));
	EXPECT_TRUE(material->colorRGBAExists(propertyName));
}

TEST(Material, FloatExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setFloat(propertyName, 25.0f);
	EXPECT_TRUE(material->floatExists(propertyName));
}

TEST(Material, IntExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setInt(propertyName, 25);
	EXPECT_TRUE(material->intExists(propertyName));
}

TEST(Material, Mat3Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat3(propertyName, glm::mat3());
	EXPECT_TRUE(material->mat3Exists(propertyName));
}

TEST(Material, Mat4Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat4(propertyName, glm::mat4());
	EXPECT_TRUE(material->mat4Exists(propertyName));
}

TEST(Material, Mat4ArrayExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat4Array(propertyName, { glm::mat4() });
	EXPECT_TRUE(material->mat4ArrayExists(propertyName));
}

TEST(Material, SubroutineValueExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	material->setSubroutine(propertyName, 25);
	EXPECT_TRUE(material->subroutineValueExists(propertyName));
}

TEST(Material, TextureExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTexture(propertyName, texture);
	EXPECT_TRUE(material->textureExists(propertyName));
}

TEST(Material, TextureSlotExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 555;
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTextureSlot(slot, texture);
	EXPECT_TRUE(material->textureSlotExists(slot));
}

TEST(Material, Vec3Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setVec3(propertyName, glm::vec3(1.0f, 2.0f, 3.0f));
	EXPECT_TRUE(material->vec3Exists(propertyName));
}

TEST(Material, Vec4Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setVec4(propertyName, glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
	EXPECT_TRUE(material->vec4Exists(propertyName));
}

TEST(Material, BoolDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->boolExists(propertyName));
}

TEST(Material, ColorRGBDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->colorRGBExists(propertyName));
}

TEST(Material, ColorRGBADoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->colorRGBAExists(propertyName));
}

TEST(Material, FloatDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->floatExists(propertyName));
}

TEST(Material, IntDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->intExists(propertyName));
}

TEST(Material, Mat3DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat3Exists(propertyName));
}

TEST(Material, Mat4DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat4Exists(propertyName));
}

TEST(Material, Mat4ArrayDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat4ArrayExists(propertyName));
}

TEST(Material, SubroutineValueDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	EXPECT_FALSE(material->subroutineValueExists(propertyName));
}

TEST(Material, TextureDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->textureExists(propertyName));
}

TEST(Material, TextureSlotDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 555;
	EXPECT_FALSE(material->textureSlotExists(slot));
}

TEST(Material, Vec3DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->vec3Exists(propertyName));
}

TEST(Material, Vec4DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->vec4Exists(propertyName));
}
