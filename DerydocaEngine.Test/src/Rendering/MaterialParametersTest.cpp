#include "EngineTestPch.h"
#include "Rendering\Material.h"
#include "Color.h"
#include "Rendering\Texture.h"

struct MaterialCopyTest : testing::Test
{
	std::shared_ptr<DerydocaEngine::Rendering::Material> sourceMaterial;
	std::shared_ptr<DerydocaEngine::Rendering::Material> newMaterial;
	std::string paramName;
	std::string paramNameAlt;
	int paramInt;
	unsigned int paramUInt;

	MaterialCopyTest() :
		sourceMaterial(std::make_shared<DerydocaEngine::Rendering::Material>()),
		newMaterial(std::make_shared<DerydocaEngine::Rendering::Material>()),
		paramName("param1"),
		paramNameAlt("param2"),
		paramInt(5),
		paramUInt(10)
	{
	}

	void copy()
	{
		newMaterial->copyFrom(sourceMaterial);
	}
};

TEST_F(MaterialCopyTest, BoolIsCopied) {
	sourceMaterial->setBool(paramName, true);
	sourceMaterial->setBool(paramNameAlt, false);

	copy();

	EXPECT_EQ(true, newMaterial->getBool(paramName));
	EXPECT_EQ(false, newMaterial->getBool(paramNameAlt));
}

TEST(Material, BoolDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(false, material->getBool(propertyName));
}

TEST(Material, BoolDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->boolExists(propertyName));
}

TEST(Material, BoolExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setBool(propertyName, true);
	EXPECT_TRUE(material->boolExists(propertyName));
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

TEST_F(MaterialCopyTest, ColorRGBAIsCopied) {
	auto value = DerydocaEngine::Color(0.1f, 0.2f, 0.3f, 0.4f);
	sourceMaterial->setColorRGBA(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getColorRGBA(paramName));
}

TEST(Material, ColorRGBADefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(DerydocaEngine::Color(), material->getColorRGBA(propertyName));
}

TEST(Material, ColorRGBADoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->colorRGBAExists(propertyName));
}

TEST(Material, ColorRGBAExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setColorRGBA(propertyName, DerydocaEngine::Color(1.0f, 0.0f, 0.0f, 1.0f));
	EXPECT_TRUE(material->colorRGBAExists(propertyName));
}

TEST(Material, ColorRGBAIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto color = DerydocaEngine::Color(0.1f, 0.2f, 0.3f, 0.4f);
	material->setColorRGBA(propertyName, color);
	EXPECT_EQ(color, material->getColorRGBA(propertyName));
}

TEST_F(MaterialCopyTest, ColorRGBIsCopied) {
	auto value = DerydocaEngine::Color(0.1f, 0.2f, 0.3f);
	sourceMaterial->setColorRGB(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getColorRGB(paramName));
}

TEST(Material, ColorRGBDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(DerydocaEngine::Color(), material->getColorRGB(propertyName));
}

TEST(Material, ColorRGBDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->colorRGBExists(propertyName));
}

TEST(Material, ColorRGBExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setColorRGB(propertyName, DerydocaEngine::Color(1.0f, 0.0f, 0.0f));
	EXPECT_TRUE(material->colorRGBExists(propertyName));
}

TEST(Material, ColorRGBIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto color = DerydocaEngine::Color(0.1f, 0.2f, 0.3f);
	material->setColorRGB(propertyName, color);
	EXPECT_EQ(color, material->getColorRGB(propertyName));
}

TEST_F(MaterialCopyTest, FloatIsCopied) {
	auto value = 25.0f;
	sourceMaterial->setFloat(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getFloat(paramName));
}

TEST(Material, FloatDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(0.0f, material->getFloat(propertyName));
}

TEST(Material, FloatDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->floatExists(propertyName));
}

TEST(Material, FloatExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setFloat(propertyName, 25.0f);
	EXPECT_TRUE(material->floatExists(propertyName));
}

TEST(Material, FloatIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	float value = 25.0f;
	material->setFloat(propertyName, value);
	EXPECT_EQ(value, material->getFloat(propertyName));
}

TEST_F(MaterialCopyTest, FloatArrayIsCopied) {
	auto value = std::vector<float>({ 5.0f, 10.0f, 25.0f });
	sourceMaterial->setFloatArray(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getFloatArray(paramName));
}

TEST(Material, FloatArrayDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(std::vector<float>(), material->getFloatArray(propertyName));
}

TEST(Material, FloatArrayDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->floatArrayExists(propertyName));
}

TEST(Material, FloatArrayExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setFloatArray(propertyName, { 5.0f, 10.0f, 25.0f });
	EXPECT_TRUE(material->floatArrayExists(propertyName));
}

TEST(Material, FloatArrayIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto value = std::vector<float>({ 5.0f, 10.0f, 25.0f });
	material->setFloatArray(propertyName, value);
	EXPECT_EQ(value, material->getFloatArray(propertyName));
}

TEST_F(MaterialCopyTest, IntIsCopied) {
	auto value = 25;
	sourceMaterial->setInt(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getInt(paramName));
}

TEST(Material, IntDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(0, material->getInt(propertyName));
}

TEST(Material, IntDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->intExists(propertyName));
}

TEST(Material, IntExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setInt(propertyName, 25);
	EXPECT_TRUE(material->intExists(propertyName));
}

TEST(Material, IntIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	int value = 25;
	material->setInt(propertyName, value);
	EXPECT_EQ(value, material->getInt(propertyName));
}

TEST_F(MaterialCopyTest, Mat3IsCopied) {
	auto value = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	sourceMaterial->setMat3(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getMat3(paramName));
}

TEST(Material, Mat3DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::mat3(), material->getMat3(propertyName));
}

TEST(Material, Mat3DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat3Exists(propertyName));
}

TEST(Material, Mat3Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat3(propertyName, glm::mat3());
	EXPECT_TRUE(material->mat3Exists(propertyName));
}

TEST(Material, Mat3IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto mat = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	material->setMat3(propertyName, mat);
	EXPECT_EQ(mat, material->getMat3(propertyName));
}

TEST_F(MaterialCopyTest, Mat4IsCopied) {
	auto value = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	sourceMaterial->setMat4(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getMat4(paramName));
}

TEST(Material, Mat4DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::mat4(), material->getMat4(propertyName));
}

TEST(Material, Mat4DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat4Exists(propertyName));
}

TEST(Material, Mat4Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat4(propertyName, glm::mat4());
	EXPECT_TRUE(material->mat4Exists(propertyName));
}

TEST(Material, Mat4IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto mat = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	material->setMat4(propertyName, mat);
	EXPECT_EQ(mat, material->getMat4(propertyName));
}

TEST_F(MaterialCopyTest, Mat4ArrayIsCopied) {
	auto value = std::vector<glm::mat4>({ glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) });
	sourceMaterial->setMat4Array(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getMat4Array(paramName));
}

TEST(Material, Mat4ArrayDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(std::vector<glm::mat4>(), material->getMat4Array(propertyName));
}

TEST(Material, Mat4ArrayDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->mat4ArrayExists(propertyName));
}

TEST(Material, Mat4ArrayExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setMat4Array(propertyName, { glm::mat4() });
	EXPECT_TRUE(material->mat4ArrayExists(propertyName));
}

TEST(Material, Mat4ArrayIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto matArr = std::vector<glm::mat4>({ glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) });
	material->setMat4Array(propertyName, matArr);
	EXPECT_EQ(matArr, material->getMat4Array(propertyName));
}

TEST_F(MaterialCopyTest, SubroutineValueArrayIsCopied) {
	unsigned int value = 22;
	sourceMaterial->setSubroutine(paramUInt, value);

	copy();

	EXPECT_EQ(value, newMaterial->getSubroutineValue(paramUInt));
}

TEST(Material, SubroutineValueDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	EXPECT_EQ(0, material->getSubroutineValue(propertyName));
}

TEST(Material, SubroutineValueDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	EXPECT_FALSE(material->subroutineValueExists(propertyName));
}

TEST(Material, SubroutineValueExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	material->setSubroutine(propertyName, 25);
	EXPECT_TRUE(material->subroutineValueExists(propertyName));
}

TEST(Material, SubroutineValueIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	unsigned int propertyName = 5;
	unsigned int value = 555;
	material->setSubroutine(propertyName, value);
	EXPECT_EQ(value, material->getSubroutineValue(propertyName));
}

TEST_F(MaterialCopyTest, TextureIsCopied) {
	auto value = std::make_shared<DerydocaEngine::Rendering::Texture>();
	sourceMaterial->setTexture(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getTexture(paramName));
}

TEST(Material, TextureDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	bool isNull = material->getTexture(propertyName) == nullptr;
	EXPECT_TRUE(isNull);
}

TEST(Material, TextureDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->textureExists(propertyName));
}

TEST(Material, TextureExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTexture(propertyName, texture);
	EXPECT_TRUE(material->textureExists(propertyName));
}

TEST(Material, TextureIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTexture(propertyName, texture);
	EXPECT_EQ(texture, material->getTexture(propertyName));
}

TEST_F(MaterialCopyTest, TextureSlotIsCopied) {
	auto value = std::make_shared<DerydocaEngine::Rendering::Texture>();
	sourceMaterial->setTextureSlot(paramInt, value);

	copy();

	EXPECT_EQ(value, newMaterial->getTextureSlot(paramInt));
}

TEST(Material, TextureSlotDefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 5;
	bool isNull = material->getTextureSlot(slot) == nullptr;
	EXPECT_TRUE(isNull);
}

TEST(Material, TextureSlotDoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 5;
	EXPECT_FALSE(material->textureSlotExists(slot));
}

TEST(Material, TextureSlotExists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 5;
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTextureSlot(slot, texture);
	EXPECT_TRUE(material->textureSlotExists(slot));
}

TEST(Material, TextureSlotIsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	int slot = 5;
	auto texture = std::make_shared<DerydocaEngine::Rendering::Texture>();
	material->setTextureSlot(slot, texture);
	EXPECT_EQ(texture, material->getTextureSlot(slot));
}

TEST_F(MaterialCopyTest, Vec3IsCopied) {
	auto value = glm::vec3(1.0f, 2.0f, 3.0f);
	sourceMaterial->setVec3(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getVec3(paramName));
}

TEST(Material, Vec3DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->vec3Exists(propertyName));
}

TEST(Material, Vec3Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setVec3(propertyName, glm::vec3(1.0f, 2.0f, 3.0f));
	EXPECT_TRUE(material->vec3Exists(propertyName));
}

TEST(Material, Vec3IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto vec = glm::vec3(1.0f, 2.0f, 3.0f);
	material->setVec3(propertyName, vec);
	EXPECT_EQ(vec, material->getVec3(propertyName));
}

TEST(Material, Vec3DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::vec3(), material->getVec3(propertyName));
}

TEST_F(MaterialCopyTest, Vec4IsCopied) {
	auto value = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
	sourceMaterial->setVec4(paramName, value);

	copy();

	EXPECT_EQ(value, newMaterial->getVec4(paramName));
}

TEST(Material, Vec4DefaultIsReturned_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_EQ(glm::vec4(), material->getVec4(propertyName));
}

TEST(Material, Vec4DoesNotExist_When_NoneIsSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	EXPECT_FALSE(material->vec4Exists(propertyName));
}

TEST(Material, Vec4Exists_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	material->setVec4(propertyName, glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
	EXPECT_TRUE(material->vec4Exists(propertyName));
}

TEST(Material, Vec4IsReturned_After_BeingSet)
{
	auto material = std::make_shared<DerydocaEngine::Rendering::Material>();

	std::string propertyName = "MyProperty";
	auto vec = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
	material->setVec4(propertyName, vec);
	EXPECT_EQ(vec, material->getVec4(propertyName));
}
