#include "Rendering\Material.h"

#include <gl\glew.h>
#include "Rendering\Shader.h"
#include "Rendering\Texture.h"
#include "Color.h"

namespace DerydocaEngine::Rendering
{

	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	void Material::setBool(std::string const& name, bool const& value)
	{
		m_boolValues[name] = value;
	}

	void Material::setInt(std::string const& name, int const& value)
	{
		m_intValues[name] = value;
	}

	void Material::setTexture(std::string const & name, Texture * const & texture)
	{
		m_textures[name] = texture;
	}

	void Material::setTextureSlot(int const& slot, Texture * const& texture)
	{
		m_texture = texture;
	}

	void Material::setFloat(std::string const& name, float const& value)
	{
		m_floatValues[name] = value;
	}

	void Material::setVec3(std::string const& name, glm::vec3 const& value)
	{
		m_vec3Values[name] = value;
	}

	void Material::setVec4(std::string const& name, glm::vec4 const& value)
	{
		m_vec4Values[name] = value;
	}

	void Material::setColorRGB(std::string const& name, Color const& value)
	{
		m_vec3Values[name] = glm::vec3(value.r, value.g, value.b);
	}

	void Material::setColorRGBA(std::string const& name, Color const& value)
	{
		m_vec4Values[name] = glm::vec4(value.r, value.g, value.b, value.a);
	}

	void Material::setMat3(std::string const& name, glm::mat3 const& value)
	{
		m_mat3Values[name] = value;
	}

	void Material::setMat4(std::string const& name, glm::mat4 const& value)
	{
		m_mat4Values[name] = value;
	}

	void Material::setMat4Array(std::string const & name, std::vector<glm::mat4> matrixArray)
	{
		m_mat4ArrayValues[name] = matrixArray;
	}

	void Material::setSubroutine(unsigned int const& program, unsigned int const& value)
	{
		m_subroutineValues[program] = value;
	}

	void Material::bind()
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

}
