#pragma once
#include <vector>
#include "TexturePackerImage.h"

class Texture;

struct TexturePackerTextureData
{
	void Resize(int x, int y);

	void AddImage(int xPos, int yPos, TexturePackerImage* image, unsigned char* imageBuffer);

	void Fill(int sx, int sy, int ex, int ey, unsigned char value);

	int getWidth() const { return (int)m_data[0].size(); }
	int getHeight() const { return (int)m_data.size(); }
	Texture* allocTexture();
	unsigned char* allocImageBuffer();
private:
	vector<vector<unsigned char>> m_data;
};
