#include "EnginePch.h"
#include "Rendering\Texture.h"

#include <cassert>

namespace DerydocaEngine::Rendering
{
	Texture::Texture() :
		m_rendererId(0),
		m_width(0),
		m_height(0),
		m_textureType(TextureType::Texture2D)
	{
	}

	Texture::Texture(const std::string& fileName) :
		m_rendererId(0),
		m_width(0),
		m_height(0),
		m_textureType(TextureType::Texture2D)
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

		updateBuffer(data, w, h, bpp, nullptr);

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
		m_textureType(TextureType::Cubemap)
	{
		auto textureType = GraphicsAPI::translate(m_textureType);

		std::string cubemapSourceImages[] = { xpos, xneg, ypos, yneg, zpos, zneg };

		std::array<char*, 6> cubemapSourceImageData;
		int w, h, bpp;
		for (unsigned int i = 0; i < 6; i++)
		{
			// Load the data
			stbi_set_flip_vertically_on_load(false);
			cubemapSourceImageData[i] = (char*)(stbi_load(cubemapSourceImages[i].c_str(), &w, &h, &bpp, 0));
		}

		GraphicsAPI::generateCubemap(m_rendererId, cubemapSourceImageData, w, h, InternalTextureFormat::RGB, TextureDataType::UnsignedByte, false);

		for (unsigned int i = 0; i < 6; i++)
		{
			stbi_image_free(cubemapSourceImageData[i]);
		}
	}

	Texture::~Texture()
	{
		deleteTexture();
	}

	void Texture::bind(const unsigned int unit) const
	{
		GraphicsAPI::bindTexture2D(m_rendererId, m_textureType, unit);
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
		m_textureType = TextureType::Texture2D;

		// delete the old texture if there was one already loaded
		deleteTexture();

		InternalTextureFormat pixelFormat = channelsToPixelFormat(channels);

		GraphicsAPI::createTexture2D(m_rendererId, m_textureType, pixelFormat, width, height, true, data);
	}

	InternalTextureFormat Texture::channelsToPixelFormat(const int numChannels) const
	{
		switch (numChannels)
		{
		case 1:
			return InternalTextureFormat::R;
		case 2:
			return InternalTextureFormat::RG;
		case 3:
			return InternalTextureFormat::RGB;
		default:
			return InternalTextureFormat::RGBA;
		}
	}

	void Texture::deleteTexture()
	{
		if (m_rendererId)
		{
			GraphicsAPI::deleteTextures(1, &m_rendererId);
			m_rendererId = 0;
		}
	}

}
