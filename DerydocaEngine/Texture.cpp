#include "Texture.h"
#include <cassert>
#include <iostream>
#include "SOIL.h"
#include "GLError.h"

Texture::Texture(const std::string& fileName)
{
	/* load an image file directly as a new OpenGL texture */
	m_texture = SOIL_load_OGL_texture
	(
		fileName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* check for an error during the load process */
	if (0 == m_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	m_textureType = GL_TEXTURE_2D;
}

Texture::Texture(const std::string & xpos, const std::string & xneg, const std::string & ypos, const std::string & yneg, const std::string & zpos, const std::string & zneg)
{
	m_texture = SOIL_load_OGL_cubemap
	(
		xpos.c_str(),
		xneg.c_str(),
		ypos.c_str(),
		yneg.c_str(),
		zpos.c_str(),
		zneg.c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);
	
	/* check for an error during the load process */
	if (0 == m_texture)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	m_textureType = GL_TEXTURE_CUBE_MAP;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_textureType, m_texture);
}
