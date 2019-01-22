#include "EnginePch.h"
#include "Rendering\Material.h"

#include <gl\glew.h>
#include "Rendering\Shader.h"
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

}
