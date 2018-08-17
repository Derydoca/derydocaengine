#pragma once
#include <vector>
#include <map>
#include "SpriteReference.h"
#include "TexturePacker.h"
#include "Texture.h"

using namespace std;

class SpriteSheet
{
public:
	SpriteSheet();
	~SpriteSheet();

	void updateTexture();
	void addSprite(string textureId);
	void saveToDisk(std::string filePath);
	void LoadFromDisk(std::string filePath);

	Texture* getTexture() { return &m_texture; }
private:
	map<int, SpriteReference> m_sprites;
	unsigned char* m_imageBuffer;
	unsigned int m_largestId;
	Texture m_texture;
};

