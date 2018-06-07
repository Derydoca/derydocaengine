#include "Texture.h"
#include <cassert>
#include <iostream>
#include "stb_image.h"

Texture::Texture()
{
	m_texture = 0;
	m_textureType = GL_TEXTURE_2D;
}

Texture::Texture(const std::string& fileName)
	: Texture(fileName, nullptr)
{

}

Texture::Texture(const std::string& fileName, TextureParameters* params)
{
	// Set the texture type
	m_textureType = GL_TEXTURE_2D;

	// Get the wrap modes
	TextureWrapMode wrapModeS = TextureWrapMode::REPEAT;
	TextureWrapMode wrapModeT = TextureWrapMode::REPEAT;
	if (params != nullptr)
	{
		wrapModeS = params->getWrapModeS();
		wrapModeT = params->getWrapModeT();
	}

	// Load the image data
	int w, h, bpp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &bpp, 0);

	// Create the texture handle and set parameters for it
	glGenTextures(1, &m_texture);
	glBindTexture(m_textureType, m_texture);
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, params->textureWrapModeToOpenGL(wrapModeS));
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, params->textureWrapModeToOpenGL(wrapModeT));
	glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// If the data was loaded
	if (data)
	{
		// Load the image in OpenGL and generate mipmaps
		GLenum pixelFormat = GL_RGB;
		if (bpp == 4)
		{
			pixelFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, w, h, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free up our memory
	stbi_image_free(data);
}

Texture::Texture(const std::string & xpos, const std::string & xneg, const std::string & ypos, const std::string & yneg, const std::string & zpos, const std::string & zneg)
{
	// Set the texture type
	m_textureType = GL_TEXTURE_CUBE_MAP;

	// Create the texture handle and set parameters for it
	glGenTextures(1, &m_texture);
	glBindTexture(m_textureType, m_texture);

	// Store the list of sides in a string array so we can easily iterate over them
	std::string* cubemapSourceImages = new std::string[6]{ xpos, xneg, ypos, yneg, zpos, zneg };

	// Loop through each side of the cubemap
	int w, h, bpp;
	for (unsigned int i = 0; i < 6; i++)
	{
		// Load the data
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(cubemapSourceImages[i].c_str(), &w, &h, &bpp, 0);

		// If the data was loaded
		if (data)
		{
			// Load the image in OpenGL and generate mipmaps
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture for cubemap" << std::endl;
		}
		// Free up our memory
		stbi_image_free(data);

		// Set our parameters
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
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

void Texture::updateBuffer(GLubyte * data, int width, int height, GLint pixelFormat, TextureParameters* params)
{
	// Set the texture type
	m_textureType = GL_TEXTURE_2D;

	// Get the wrap modes
	TextureWrapMode wrapModeS = TextureWrapMode::REPEAT;
	TextureWrapMode wrapModeT = TextureWrapMode::REPEAT;
	if (params != nullptr)
	{
		wrapModeS = params->getWrapModeS();
		wrapModeT = params->getWrapModeT();
	}

	// Create the texture handle and set parameters for it
	glGenTextures(1, &m_texture);
	glBindTexture(m_textureType, m_texture);
	glTexStorage2D(m_textureType, 1, GL_RGBA8, width, height);
	glTexSubImage2D(m_textureType, 0, 0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, params->textureWrapModeToOpenGL(wrapModeS));
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, params->textureWrapModeToOpenGL(wrapModeT));
	glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
