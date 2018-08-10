#pragma once
#include <vector>
#include <map>
#include "Rectangle.h"
#include "TexturePackerImage.h"
#include "TexturePackerTextureData.h"
#include "IntRect.h"

using namespace std;

class Texture;

class TexturePacker
{
public:
	TexturePacker();
	~TexturePacker();

	void addImage(unsigned long id, float sizeX, float sizeY, float bearingX, float bearingY, float advanceX, float advanceY, unsigned char* imageBuffer, int width, int height);
	void packImages();
	bool getIsDirty() const { return m_isDirty; }
	Texture* allocTexture() { return m_packedImageData.allocTexture(); };
	unsigned char* allocImageBuffer() { return m_packedImageData.allocImageBuffer(); }
	void freeSubImageData();
	vector<TexturePackerImage> getSubImageData() { return m_images; }
	int getWidth() const { return m_packedImageData.getWidth(); }
	int getHeight() const { return m_packedImageData.getHeight(); }
private:
	vector<TexturePackerImage> m_images;
	map<unsigned long, unsigned char*> m_imageBuffers;
	bool m_isDirty;
	TexturePackerTextureData m_packedImageData;

	int getIntersectingImageXAdvance(vector<IntRect> imageBounds, IntRect rect);
};
