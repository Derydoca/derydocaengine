#pragma once
#include <vector>
#include <map>
#include "SpriteReference.h"
#include "TexturePacker.h"
#include "Texture.h"

class SpriteSheet
{
public:
	SpriteSheet();
	~SpriteSheet();

	void updateTexture();
	void addSprite(std::string const& textureId);
	void saveToDisk(std::string const& filePath);
	void LoadFromDisk(std::string const& filePath);

	Texture* getTexture() { return &m_texture; }
	SpriteReference* getSpriteReference(unsigned int const& id)
	{
		auto spriteMapRecord = m_sprites.find(id);

		if (spriteMapRecord == m_sprites.end())
		{
			return nullptr;
		}

		return &(*spriteMapRecord).second;
	}
private:
	std::map<int, SpriteReference> m_sprites;
	unsigned char* m_imageBuffer;
	unsigned int m_largestId;
	Texture m_texture;
};

