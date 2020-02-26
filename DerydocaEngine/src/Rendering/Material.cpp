#include "EnginePch.h"
#include "Rendering\Material.h"

#include <gl\glew.h>
#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Texture.h"
#include "Color.h"

namespace DerydocaEngine::Rendering
{

	Material::Material() :
		m_shader(),
		m_texture(nullptr),
		m_boolValues(),
		m_intValues(),
		m_textures(),
		m_floatValues(),
		m_vec3Values(),
		m_vec4Values(),
		m_mat3Values(),
		m_mat4Values(),
		m_mat4ArrayValues(),
		m_subroutineValues()
	{
	}

	Material::~Material()
	{
	}

	void Material::setShader(std::shared_ptr<Shader> shader)
	{
		if (!shader)
		{
			D_LOG_ERROR("Unable to set a shader because the shader reference is null.");
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}
		else if (!shader->isValid())
		{
			D_LOG_ERROR("Unable to set a shader because it is invalid.\nVert:{}\nFrag:{}", shader->GetVertexShaderPath(), shader->GetFragmentShaderPath());
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}
		m_shader = shader;
	}

	void Material::bind() const
	{
		assert(m_shader);

		// TODO: Remove this texture binding method
		m_shader->bind();
		if (m_texture != NULL)
		{
			m_texture->bind(0);
		}

		{
			int texIndex = 0;
			for (auto const& x : m_textures)
			{
				m_shader->setTexture(x.first, texIndex++, x.second);
			}
		}

		for (auto const& x : m_floatValues)
		{
			m_shader->setFloat(x.first, x.second);
		}

		for (auto const& x : m_floatArrayValues)
		{
			m_shader->setFloatArray(x.first, x.second);
		}

		for (auto const& x : m_intValues)
		{
			m_shader->setInt(x.first, x.second);
		}

		for (auto const& x : m_vec3Values)
		{
			m_shader->setVec3(x.first, x.second);
		}

		for (auto const& x : m_vec4Values)
		{
			m_shader->setVec4(x.first, x.second);
		}

		for (auto const& x : m_mat3Values)
		{
			m_shader->setMat3(x.first, x.second);
		}

		for (auto const& x : m_mat4Values)
		{
			m_shader->setMat4(x.first, x.second);
		}

		for (auto const& x : m_mat4ArrayValues)
		{
			m_shader->setMat4Array(x.first, x.second);
		}

		for (auto const& x : m_subroutineValues)
		{
			m_shader->setSubroutine(x.first, x.second);
		}

	}

	void Material::copyFrom(std::shared_ptr<Material> other)
	{
		m_boolValues = other->m_boolValues;
		m_floatValues = other->m_floatValues;
		m_floatArrayValues = other->m_floatArrayValues;
		m_intValues = other->m_intValues;
		m_mat3Values = other->m_mat3Values;
		m_mat4Values = other->m_mat4Values;
		m_mat4ArrayValues = other->m_mat4ArrayValues;
		m_subroutineValues = other->m_subroutineValues;
		m_textures = other->m_textures;
		m_texture = other->m_texture;
		m_vec3Values = other->m_vec3Values;
		m_vec4Values = other->m_vec4Values;
		m_shader = other->m_shader;
	}

	void Material::unbind()
	{
		assert(m_shader);

		{
			int texIndex = 0;
			for (auto const& x : m_textures)
			{
				m_shader->clearTexture(x.first, texIndex++, x.second->getTextureType());
			}
		}

		for (auto const& x : m_floatValues)
		{
			m_shader->clearFloat(x.first);
		}

		for (auto const& x : m_intValues)
		{
			m_shader->clearInt(x.first);
		}

		for (auto const& x : m_vec3Values)
		{
			m_shader->clearVec3(x.first);
		}

		for (auto const& x : m_vec4Values)
		{
			m_shader->clearVec4(x.first);
		}

		for (auto const& x : m_mat3Values)
		{
			m_shader->clearMat3(x.first);
		}

		for (auto const& x : m_mat4Values)
		{
			m_shader->clearMat4(x.first);
		}
	}

	void Material::setBool(const std::string& name, bool const& value)
	{
		m_boolValues[name] = value;
	}

	void Material::setColorRGB(const std::string& name, Color const& value)
	{
		m_vec3Values[name] = glm::vec3(value.r, value.g, value.b);
	}

	void Material::setColorRGBA(const std::string& name, Color const& value)
	{
		m_vec4Values[name] = glm::vec4(value.r, value.g, value.b, value.a);
	}

	void Material::setFloat(const std::string& name, float const& value)
	{
		m_floatValues[name] = value;
	}

	void Material::setFloatArray(const std::string & name, std::vector<float> value)
	{
		m_floatArrayValues[name] = value;
	}

	void Material::setInt(const std::string& name, int const& value)
	{
		m_intValues[name] = value;
	}

	void Material::setMat3(const std::string& name, glm::mat3 const& value)
	{
		m_mat3Values[name] = value;
	}

	void Material::setMat4(const std::string& name, glm::mat4 const& value)
	{
		m_mat4Values[name] = value;
	}

	void Material::setMat4Array(const std::string& name, std::vector<glm::mat4> matrixArray)
	{
		m_mat4ArrayValues[name] = matrixArray;
	}

	void Material::setSubroutine(unsigned int program, unsigned int value)
	{
		m_subroutineValues[program] = value;
	}

	void Material::setTexture(const std::string& name, std::shared_ptr<Texture> texture)
	{
		m_textures[name] = texture;
	}

	void Material::setTextureSlot(int const& slot, std::shared_ptr<Texture> texture)
	{
		m_texture = texture;
	}

	void Material::setVec3(const std::string& name, glm::vec3 const& value)
	{
		m_vec3Values[name] = value;
	}

	void Material::setVec4(const std::string& name, glm::vec4 const& value)
	{
		m_vec4Values[name] = value;
	}

	bool Material::boolExists(const std::string& name)
	{
		return m_boolValues.find(name) != m_boolValues.end();
	}

	bool Material::colorRGBExists(const std::string& name)
	{
		return m_vec3Values.find(name) != m_vec3Values.end();
	}

	bool Material::colorRGBAExists(const std::string& name)
	{
		return m_vec4Values.find(name) != m_vec4Values.end();
	}

	bool Material::floatExists(const std::string& name)
	{
		return m_floatValues.find(name) != m_floatValues.end();
	}

	bool Material::floatArrayExists(const std::string & name)
	{
		return m_floatArrayValues.find(name) != m_floatArrayValues.end();
	}

	bool Material::intExists(const std::string& name)
	{
		return m_intValues.find(name) != m_intValues.end();
	}

	bool Material::mat3Exists(const std::string& name)
	{
		return m_mat3Values.find(name) != m_mat3Values.end();
	}

	bool Material::mat4Exists(const std::string& name)
	{
		return m_mat4Values.find(name) != m_mat4Values.end();
	}

	bool Material::mat4ArrayExists(const std::string& name)
	{
		return m_mat4ArrayValues.find(name) != m_mat4ArrayValues.end();
	}

	bool Material::subroutineValueExists(unsigned int program)
	{
		return m_subroutineValues.find(program) != m_subroutineValues.end();
	}

	bool Material::textureExists(const std::string& name)
	{
		return m_textures.find(name) != m_textures.end();
	}

	bool Material::textureSlotExists(int slot)
	{
		return m_texture != nullptr;
	}

	bool Material::vec3Exists(const std::string& name)
	{
		return m_vec3Values.find(name) != m_vec3Values.end();
	}

	bool Material::vec4Exists(const std::string& name)
	{
		return m_vec4Values.find(name) != m_vec4Values.end();
	}

	bool Material::getBool(const std::string& name)
	{
		auto it = m_boolValues.find(name);
		if (it == m_boolValues.end())
		{
			return false;
		}
		else
		{
			return (*it).second;
		}
	}

	Color Material::getColorRGB(const std::string& name)
	{
		auto it = m_vec3Values.find(name);
		if (it == m_vec3Values.end())
		{
			return Color();
		}
		else
		{
			return (*it).second;
		}
	}

	Color Material::getColorRGBA(const std::string& name)
	{
		auto it = m_vec4Values.find(name);
		if (it == m_vec4Values.end())
		{
			return Color();
		}
		else
		{
			return (*it).second;
		}
	}

	float Material::getFloat(const std::string& name)
	{
		auto it = m_floatValues.find(name);
		if (it == m_floatValues.end())
		{
			return 0.0f;
		}
		else
		{
			return (*it).second;
		}
	}

	std::vector<float> Material::getFloatArray(const std::string & name)
	{
		auto it = m_floatArrayValues.find(name);
		if (it == m_floatArrayValues.end())
		{
			return std::vector<float>();
		}
		else
		{
			return (*it).second;
		}
	}

	int Material::getInt(const std::string& name)
	{
		auto it = m_intValues.find(name);
		if (it == m_intValues.end())
		{
			return 0;
		}
		else
		{
			return (*it).second;
		}
	}

	glm::mat3 Material::getMat3(const std::string& name)
	{
		auto it = m_mat3Values.find(name);
		if (it == m_mat3Values.end())
		{
			return glm::mat3();
		}
		else
		{
			return (*it).second;
		}
	}

	glm::mat4 Material::getMat4(const std::string& name)
	{
		auto it = m_mat4Values.find(name);
		if (it == m_mat4Values.end())
		{
			return glm::mat4();
		}
		else
		{
			return (*it).second;
		}
	}

	std::vector<glm::mat4> Material::getMat4Array(const std::string& name)
	{
		auto it = m_mat4ArrayValues.find(name);
		if (it == m_mat4ArrayValues.end())
		{
			return std::vector<glm::mat4>();
		}
		else
		{
			return (*it).second;
		}
	}

	unsigned int Material::getSubroutineValue(unsigned int program)
	{
		auto it = m_subroutineValues.find(program);
		if (it == m_subroutineValues.end())
		{
			return 0;
		}
		else
		{
			return (*it).second;
		}
	}

	std::shared_ptr<Texture> Material::getTexture(const std::string& name)
	{
		auto it = m_textures.find(name);
		if (it == m_textures.end())
		{
			return nullptr;
		}
		else
		{
			return (*it).second;
		}
	}

	std::shared_ptr<Texture> Material::getTextureSlot(int slot)
	{
		return m_texture;
	}

	glm::vec3 Material::getVec3(const std::string& name)
	{
		auto it = m_vec3Values.find(name);
		if (it == m_vec3Values.end())
		{
			return glm::vec3();
		}
		else
		{
			return (*it).second;
		}
	}

	glm::vec4 Material::getVec4(const std::string& name)
	{
		auto it = m_vec4Values.find(name);
		if (it == m_vec4Values.end())
		{
			return glm::vec4();
		}
		else
		{
			return (*it).second;
		}
	}

}
