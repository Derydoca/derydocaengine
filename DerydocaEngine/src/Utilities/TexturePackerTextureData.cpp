#pragma once
#include <assert.h>
#include "Utilities\TexturePackerTextureData.h"
#include "Rendering\Texture.h"
#include <climits>
#include <algorithm>

namespace DerydocaEngine::Utilities
{

	void TexturePackerTextureData::Resize(int const& x, int const& y)
	{
		m_data.resize(y);
		for (int i = 0; i < y; i++)
		{
			m_data[i].resize(x * m_channels);
		}
	}

	void TexturePackerTextureData::AddImage(int const& xPos, int const& yPos, TexturePackerImage * const& image, unsigned char* const& imageBuffer)
	{
		assert(image->getWidth() + xPos <= (int)m_data[0].size());
		assert(image->getHeight() + yPos <= (int)m_data.size());

		int channelsToRead = glm::min(image->getChannels(), m_channels);

		for (int y = 0; y < image->getHeight(); y++)
		{
			for (int x = 0; x < image->getWidth(); x++)
			{
				for (int c = 0; c < m_channels; c++)
				{
					int dataX = y + yPos;
					int dataY = ((x + xPos) * m_channels) + c;

					if (c < channelsToRead)
					{
						m_data[dataX][dataY] = imageBuffer[(y * image->getWidth() * image->getChannels()) + (x * image->getChannels()) + c];
					}
					else
					{
						m_data[dataX][dataY] = UCHAR_MAX;
					}
				}
			}
		}
	}

	void TexturePackerTextureData::Fill(int const& sx, int const& sy, int const& ex, int const& ey, unsigned char const& value)
	{
		for (int y = sy; y < ey; y++)
		{
			fill(m_data[y].begin() + sx, m_data[y].begin() + (ex * m_channels), value);
		}
	}

	std::shared_ptr<Rendering::Texture> TexturePackerTextureData::allocTexture()
	{
		// Create the image buffer
		unsigned char* dataBuffer = allocImageBuffer();

		// Load it into a texture
		auto tex = std::make_shared<Rendering::Texture>();
		tex->updateBuffer(dataBuffer, getWidth(), getHeight(), getChannels(), nullptr);

		// Free up the memory we used
		delete[] dataBuffer;

		// Return the texture
		return tex;
	}

	unsigned char * TexturePackerTextureData::allocImageBuffer()
	{
		// Create the image buffer
		unsigned char* dataBuffer = new unsigned char[getWidth() * getHeight() * getChannels()];
		for (int y = 0; y < getHeight(); y++)
		{
			memcpy(dataBuffer + (y * getWidth() * getChannels()), m_data[y].data(), getWidth() * getChannels());
		}

		return dataBuffer;
	}

}
