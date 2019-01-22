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

	int slot = 5;
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

	int slot = 5;
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

TEST(Material, BoolIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName1 = "MyProperty_true";
	std::string propertyName2 = "MyProperty_false";
	material->setBool(propertyName1, true);
	material->setBool(propertyName2, false);
	EXPECT_EQ(true, material->getBool(propertyName1));
	EXPECT_EQ(false, material->getBool(propertyName2));
}

TEST(Material, ColorRGBIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto color = DerydocaEngine::Color(0.1f, 0.2f, 0.3f);
	material->setColorRGB(propertyName, color);
	EXPECT_EQ(color, material->getColorRGB(propertyName));
}

TEST(Material, ColorRGBAIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto color = DerydocaEngine::Color(0.1f, 0.2f, 0.3f, 0.4f);
	material->setColorRGBA(propertyName, color);
	EXPECT_EQ(color, material->getColorRGBA(propertyName));
}

TEST(Material, FloatIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	float value = 25.0f;
	material->setFloat(propertyName, value);
	EXPECT_EQ(value, material->getFloat(propertyName));
}

TEST(Material, IntIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	int value = 25;
	material->setInt(propertyName, value);
	EXPECT_EQ(value, material->getInt(propertyName));
}

TEST(Material, Mat3IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto mat = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	material->setMat3(propertyName, mat);
	EXPECT_EQ(mat, material->getMat3(propertyName));
}

TEST(Material, Mat4IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto mat = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	material->setMat4(propertyName, mat);
	EXPECT_EQ(mat, material->getMat4(propertyName));
}

TEST(Material, Mat4ArrayIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto matArr = std::vector<glm::mat4>({ glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) });
	material->setMat4Array(propertyName, matArr);
	EXPECT_EQ(matArr, material->getMat4Array(propertyName));
}

TEST(Material, SubroutineValueIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	unsigned int value = 555;
	material->setSubroutine(propertyName, value);
	EXPECT_EQ(value, material->getSubroutineValue(propertyName));
}

TEST(Material, TextureIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();
	
	std::string propertyName = "MyProperty";
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTexture(propertyName, texture);
	EXPECT_EQ(texture, material->getTexture(propertyName));
}

TEST(Material, TextureSlotIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();
	
	int slot = 5;
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTextureSlot(slot, texture);
	EXPECT_EQ(texture, material->getTextureSlot(slot));
}

TEST(Material, Vec3IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto vec = glm::vec3(1.0f, 2.0f, 3.0f);
	material->setVec3(propertyName, vec);
	EXPECT_EQ(vec, material->getVec3(propertyName));
}

TEST(Material, Vec4IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto vec = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
	material->setVec4(propertyName, vec);
	EXPECT_EQ(vec, material->getVec4(propertyName));
}

TEST(Material, BoolDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(false, material->getBool(propertyName));
}

TEST(Material, ColorRGBDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(DerydocaEngine::Color(), material->getColorRGB(propertyName));
}

TEST(Material, ColorRGBADefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(DerydocaEngine::Color(), material->getColorRGBA(propertyName));
}

TEST(Material, FloatDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(0.0f, material->getFloat(propertyName));
}

TEST(Material, IntDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(0, material->getInt(propertyName));
}

TEST(Material, Mat3DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::mat3(), material->getMat3(propertyName));
}

TEST(Material, Mat4DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::mat4(), material->getMat4(propertyName));
}

TEST(Material, Mat4ArrayDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(std::vector<glm::mat4>(), material->getMat4Array(propertyName));
}

TEST(Material, SubroutineValueDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	EXPECT_EQ(0, material->getSubroutineValue(propertyName));
}

TEST(Material, TextureDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	bool isNull = material->getTexture(propertyName) == nullptr;
	EXPECT_TRUE(isNull);
}

TEST(Material, TextureSlotDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 5;
	bool isNull = material->getTextureSlot(slot) == nullptr;
	EXPECT_TRUE(isNull);
}

TEST(Material, Vec3DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::vec3(), material->getVec3(propertyName));
}

TEST(Material, Vec4DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::vec4(), material->getVec4(propertyName));
}
