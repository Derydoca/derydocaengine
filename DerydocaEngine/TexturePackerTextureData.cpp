#pragma once
#include <assert.h>
#include "TexturePackerTextureData.h"
#include "Texture.h"

void TexturePackerTextureData::Resize(int x, int y)
{
	m_data.resize(y);
	for (int i = 0; i < y; i++)
	{
		m_data[i].resize(x);
	}
}

void TexturePackerTextureData::AddImage(int xPos, int yPos, TexturePackerImage * image, unsigned char* imageBuffer)
{
	assert(image->getWidth() + xPos <= (int)m_data[0].size());
	assert(image->getHeight() + yPos <= (int)m_data.size());

	for (int y = 0; y < image->getHeight(); y++)
	{
		for (int x = 0; x < image->getWidth(); x++)
		{
			m_data[y + yPos][x + xPos] = imageBuffer[(y * image->getWidth()) + x];
		}
	}
}

void TexturePackerTextureData::Fill(int sx, int sy, int ex, int ey, unsigned char value)
{
	for (int y = sy; y < ey; y++)
	{
		fill(m_data[y].begin() + sx, m_data[y].begin() + ex, value);
	}
}

Texture * TexturePackerTextureData::allocTexture()
{
	// Create the image buffer
	unsigned char* dataBuffer = allocImageBuffer();

	// Load it into a texture
	Texture* tex = new Texture();
	tex->updateBuffer(dataBuffer, getWidth(), getHeight(), GL_RED, nullptr);

	// Free up the memory we used
	delete[] dataBuffer;

	// Return the texture
	return tex;
}

unsigned char * TexturePackerTextureData::allocImageBuffer()
{
	// Create the image buffer
	unsigned char* dataBuffer = new unsigned char[getWidth() * getHeight()];
	for (int y = 0; y < getHeight(); y++)
	{
		memcpy(dataBuffer + (y * getWidth()), m_data[y].data(), getWidth());
	}

	return dataBuffer;
}
