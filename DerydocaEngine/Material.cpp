#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material()
{
}


Material::~Material()
{
}

void Material::setTexture(const std::string name, Texture* texture)
{
	m_textures[name] = texture;
}

void Material::setTextureSlot(int slot, Texture * texture)
{
	m_texture = texture;
}

void Material::setFloat(const std::string name, float value)
{
	m_floatValues[name] = value;
}

void Material::setVec3(const std::string name, glm::vec3 value)
{
	m_vec3Values[name] = value;
}

void Material::setVec4(const std::string name, glm::vec4 value)
{
	m_vec4Values[name] = value;
}

void Material::setColorRGB(const std::string name, Color value)
{
	m_vec3Values[name] = glm::vec3(value.r, value.g, value.b);
}

void Material::setColorRGBA(const std::string name, Color value)
{
	m_vec4Values[name] = glm::vec4(value.r, value.g, value.b, value.a);
}

void Material::setMat3(const std::string name, glm::mat3 value)
{
	m_mat3Values[name] = value;
}

void Material::setMat4(const std::string name, glm::mat4 value)
{
	m_mat4Values[name] = value;
}

void Material::setSubroutine(const GLuint program, const GLuint value)
{
	m_subroutineValues[program] = value;
}

void Material::bind()
{
	assert(m_shader);
	//assert(m_texture);

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

	for (auto const& x : m_subroutineValues)
	{
		m_shader->setSubroutine(x.first, x.second);
	}

}
