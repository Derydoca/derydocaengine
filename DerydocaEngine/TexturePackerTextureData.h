#pragma once
#include <vector>
#include "TexturePackerImage.h"

class Texture;

struct TexturePackerTextureData
{
	void Resize(int x, int y);

	void AddImage(int xPos, int yPos, TexturePackerImage* image, unsigned char* imageBuffer);

	void Fill(int sx, int sy, int ex, int ey, unsigned char value);

	int getWidth() const { return m_data[0].size(); }
	int getHeight() const { return m_data.size(); }
	unsigned char get(int x, int y) const {
		if (y < 0 || y >= (int)m_data.size() || x < 0 || x >= (int)m_data[0].size())
		{
			return 0;
		}
		return m_data[y][x];
	}
	Texture* allocTexture();
	unsigned char* allocImageBuffer();
private:
	vector<vector<unsigned char>> m_data;
};
