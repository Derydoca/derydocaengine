#include "EnginePch.h"
#include "Rendering\Texture.h"

#include <cassert>
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

	Texture::Texture(const std::string& fileName, const TextureParameters* params) :
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
			D_LOG_ERROR("Unable to load image.");
			return;
		}

		updateBuffer(data, w, h, bpp, params);

		// Free up our memory
		stbi_image_free(data);
	}

	Texture::Texture(
		const std::string& xpos,
		const std::string& xneg,
		const std::string& ypos,
		const std::string& yneg,
		const std::string& zpos,
		const std::string& zneg
	) :
		m_rendererId(0),
		m_width(0),
		m_height(0),
		m_textureType(GL_TEXTURE_CUBE_MAP)
	{
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
				D_LOG_ERROR("Failed to load texture for cubemap.");
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

	void Texture::bind(const unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_textureType, m_rendererId);
	}

	void Texture::updateBuffer(
		unsigned char * data,
		const int width,
		const int height,
		const int channels,
		const TextureParameters* params
	)
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
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, TextureParameters::textureWrapModeToOpenGL(wrapModeS));
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, TextureParameters::textureWrapModeToOpenGL(wrapModeT));
		glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(m_textureType);
	}

	unsigned int Texture::channelsToPixelFormat(const int numChannels) const
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
