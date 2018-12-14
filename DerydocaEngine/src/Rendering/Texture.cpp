#include "EnginePch.h"
#include "Rendering\Texture.h"

#include <cassert>
#include <GL/glew.h>
#include <iostream>
#include "vendor/stb_image.h"
#include "Rendering\TextureParameters.h"

namespace DerydocaEngine::Rendering
{

	Texture::Texture() :
		m_rendererId(0),
		m_width(0),
		m_height(0),
		m_textureType(0)
	{
		m_textureType = GL_TEXTURE_2D;
	}

	Texture::Texture(std::string const& fileName, TextureParameters* const& params) :
		m_rendererId(0),
		m_width(0),
		m_height(0),
		m_textureType(0)
	{
		// Load the image data
		int w, h, bpp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &bpp, 0);
		if (!data)
		{
			std::cout << "Unable to load image.\n";
			return;
		}

		updateBuffer(data, w, h, bpp, params);

		// Free up our memory
		stbi_image_free(data);
	}

	Texture::Texture(std::string const& xpos, std::string const& xneg, std::string const& ypos, std::string const& yneg, std::string const& zpos, std::string const& zneg)
	{
		// Set the texture type
		m_textureType = GL_TEXTURE_CUBE_MAP;

		// Create the texture handle and set parameters for it
		glGenTextures(1, &m_rendererId);
		glBindTexture(m_textureType, m_rendererId);

		// Store the list of sides in a string array so we can easily iterate over them
		std::string cubemapSourceImages[] = { xpos, xneg, ypos, yneg, zpos, zneg };

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
				std::cout << "Failed to load texture for cubemap\n";
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
		deleteTexture();
	}

	void Texture::bind(unsigned int const& unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_textureType, m_rendererId);
	}

	void Texture::updateBuffer(unsigned char * const& data, int const& width, int const& height, int const& channels, TextureParameters* const& params)
	{
		m_width = width;
		m_height = height;

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

		GLint pixelFormat = channelsToPixelFormat(channels);

		if (pixelFormat == GL_RED)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}

		// delete the old texture if there was one already loaded
		deleteTexture();
		
		// Create the texture handle and set parameters for it
		glGenTextures(1, &m_rendererId);
		glBindTexture(m_textureType, m_rendererId);
		glTexImage2D(m_textureType, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, params->textureWrapModeToOpenGL(wrapModeS));
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, params->textureWrapModeToOpenGL(wrapModeT));
		glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(m_textureType);
	}

	unsigned int Texture::channelsToPixelFormat(int const & numChannels) const
	{
		switch (numChannels)
		{
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		default:
			return GL_RGBA;
		}
	}

	void Texture::deleteTexture()
	{
		if (m_rendererId)
		{
			glDeleteTextures(1, &m_rendererId);
			m_rendererId = 0;
		}
	}

}
