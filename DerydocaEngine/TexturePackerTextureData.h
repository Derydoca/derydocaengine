#pragma once
#include <vector>
#include <assert.h>
#include "TexturePackerImage.h"

class Texture;

struct TexturePackerTextureData
{
	TexturePackerTextureData()
	{
		m_channels = 4;
	}

	TexturePackerTextureData(int channels)
	{
		assert(channels > 0);
		m_channels = channels;
	}

	void Resize(int x, int y);

	void AddImage(int xPos, int yPos, TexturePackerImage* image, unsigned char* imageBuffer);

	void Fill(int sx, int sy, int ex, int ey, unsigned char value);

	int getWidth() const { return (int)m_data[0].size() / m_channels; }
	int getHeight() const { return (int)m_data.size(); }
	int getChannels() const { return m_channels; }
	Texture* allocTexture();
	unsigned char* allocImageBuffer();
private:
	int m_channels;
	vector<vector<unsigned char>> m_data;
};
