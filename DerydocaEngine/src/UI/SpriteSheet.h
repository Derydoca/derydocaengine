#pragma once
#include <vector>
#include <map>
#include "UI\SpriteReference.h"
#include "Utilities\TexturePacker.h"
#include "Rendering\Texture.h"

namespace DerydocaEngine::UI
{

	class SpriteSheet
	{
	public:
		SpriteSheet();
		~SpriteSheet();

		void updateTexture();
		void addSprite(std::string const& textureId);
		void saveToDisk(const std::string& filePath);
		void LoadFromDisk(std::string const& filePath);
		void LoadFromSerializedFile(const std::string& filePath);
		void SaveToSerializedFile(const std::string& filePath);

		std::shared_ptr<Rendering::Texture> getTexture() { return m_texture; }
		SpriteReference* getSpriteReference(unsigned int const& id)
		{
			auto spriteMapRecord = m_sprites.find(id);

			if (spriteMapRecord == m_sprites.end())
			{
				return nullptr;
			}

			return &(*spriteMapRecord).second;
		}

		SERIALIZE_FUNC(
			SERIALIZE(m_sprites),
			SERIALIZE(m_texture->m_id)
		);
	private:
		std::map<int, SpriteReference> m_sprites;
		unsigned char* m_imageBuffer;
		unsigned int m_largestId;
		std::shared_ptr<Rendering::Texture> m_texture;
	};

}
