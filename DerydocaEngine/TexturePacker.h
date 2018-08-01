#pragma once
#include <vector>
#include <map>
#include "Rectangle.h"
#include "TexturePackerImage.h"
#include "TexturePackerTextureData.h"
#include "IntRectangle.h"

using namespace std;

class Texture;

class TexturePacker
{
public:
	TexturePacker();
	~TexturePacker();

	void addImage(int id, float sizeX, float sizeY, float bearingX, float bearingY, float advanceX, float advanceY, unsigned char* imageBuffer, int width, int height);
	void packImages();
	bool getIsDirty() const { return m_isDirty; }
	Texture* allocTexture() { return dest.allocTexture(); };
	unsigned char* allocImageBuffer() { return dest.allocImageBuffer(); }
	void freeSubImageData();
	vector<TexturePackerImage> getSubImageData() { return m_images; }
	int getWidth() const { return dest.getWidth(); }
	int getHeight() const { return dest.getHeight(); }
private:
	const int IMAGE_SIZE = 512;
	vector<TexturePackerImage> m_images;
	map<int, unsigned char*> m_imageBuffers;
	bool m_isDirty;
	TexturePackerTextureData dest;
	vector<IntRectangle> m_imageBounds;

	bool isPointFree(int x, int y);
	bool isRectFree(int x, int y, int w, int h);
};
