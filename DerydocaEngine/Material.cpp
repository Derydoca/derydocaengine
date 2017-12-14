#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material()
{
}


Material::~Material()
{
}

void Material::setTextureSlot(int slot, Texture * texture)
{
	m_texture = texture;
}

void Material::bind()
{
	assert(m_shader);
	assert(m_texture);

	m_texture->bind(0);
	m_shader->bind();
}
