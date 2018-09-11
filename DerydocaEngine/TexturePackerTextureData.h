#pragma once
#include <vector>
#include <assert.h>
#include "TexturePackerImage.h"

class Texture;

namespace DerydocaEngine::Utilities
{

	struct TexturePackerTextureData
	{
		TexturePackerTextureData()
		{
			m_channels = 4;
		}

		TexturePackerTextureData(int const& channels)
		{
			assert(channels > 0);
			m_channels = channels;
		}

		void Resize(int const& x, int const& y);

		void AddImage(int const& xPos, int const& yPos, TexturePackerImage* const& image, unsigned char* const& imageBuffer);

		void Fill(int const& sx, int const& sy, int const& ex, int const& ey, unsigned char const& value);

		int getWidth() const { return (int)m_data[0].size() / m_channels; }
		int getHeight() const { return (int)m_data.size(); }
		int getChannels() const { return m_channels; }
		Texture* allocTexture();
		unsigned char* allocImageBuffer();
	private:
		int m_channels;
		std::vector<std::vector<unsigned char>> m_data;
	};

}
