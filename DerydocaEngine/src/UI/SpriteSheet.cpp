#include "EnginePch.h"
#include "UI\Spritesheet.h"
#include "ObjectLibrary.h"
#include <boost\lexical_cast.hpp>
#include <fstream>
#include "Files\FileUtils.h"
#include "AssetData\SpriteSheetData.h"

namespace DerydocaEngine::UI
{

	SpriteSheet::SpriteSheet() :
		m_sprites(),
		m_imageBuffer(),
		m_largestId(0),
		m_texture(std::make_shared<Rendering::Texture>())
	{
		m_sprites = std::map<int, SpriteReference>();
	}

	SpriteSheet::~SpriteSheet()
	{
	}

	void SpriteSheet::updateTexture()
	{
		Utilities::TexturePacker packer = Utilities::TexturePacker(4);
		ObjectLibrary* ObjLib = &ObjectLibrary::getInstance();

		for (auto spriteMapItem : m_sprites)
		{
			SpriteReference sprite = spriteMapItem.second;

			auto spriteImageResource = ObjLib->getResource(sprite.textureId);

			int imgw, imgh, imgch;
			unsigned char* spriteImageBuffer = stbi_load(spriteImageResource->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
			sprite.size = { imgw, imgh };
			sprite.type = SpriteType::Sprite;
			packer.addImage(sprite.id, { 0, 0 }, { 0, 0 }, spriteImageBuffer, { imgw, imgh }, imgch);
			delete[] spriteImageBuffer;

			m_sprites[spriteMapItem.first] = sprite;
		}

		packer.packImages();

		std::vector<Utilities::TexturePackerImage> packedImages = packer.getSubImageData();
		for (auto img : packedImages)
		{
			auto spriteMapRecord = m_sprites.find(img.id);

			if (spriteMapRecord == m_sprites.end())
			{
				continue;
			}

			auto tex = img.imageRect;
			(*spriteMapRecord).second.texPosition.set(tex.getX(), tex.getY(), tex.getDX(), tex.getDY());
		}

		delete[] m_imageBuffer;
		m_imageBuffer = packer.allocImageBuffer();
		m_texture->updateBuffer(m_imageBuffer, packer.getWidth(), packer.getHeight(), packer.getChannels(), nullptr);
	}

	void SpriteSheet::addSprite(std::string const& textureId)
	{
		SpriteReference sprite = SpriteReference(++m_largestId);
		sprite.textureId = textureId;
		sprite.type = SpriteType::Sprite;
		m_sprites[m_largestId] = sprite;
	}

    void SpriteSheet::LoadFromSerializedFile(const std::string& filePath)
    {
		auto data = Files::Utils::ReadFromDisk<AssetData::SpriteSheetData>(filePath);

		for (auto img : data.sprites)
		{
			m_sprites.emplace(img.id, img);
		}

		auto r = ObjectLibrary::getInstance().getResource(data.textureId);
		int imgw, imgh, imgch;
		unsigned char* imageData = stbi_load(r->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
		int2 imageSize = { imgw, imgh };
		delete[] m_imageBuffer;
		m_imageBuffer = new unsigned char[imageSize.x * imageSize.y];
		for (int i = 0; i < imageSize.x * imageSize.y; i++)
		{
			m_imageBuffer[i] = imageData[i * imgch];
		}
		m_imageBuffer = imageData;
		m_texture->updateBuffer(m_imageBuffer, imgw, imgh, imgch, nullptr);
		delete[] imageData;

		//m_textureDirty = true;
    }

	void SpriteSheet::SaveToSerializedFile(const std::string& filePath)
	{
		// Write the generated texture to disk
		std::string imageFileName = filePath + ".bmp";
		int imgw = 0, imgh = 0, imgch = 0;
		stbi_write_bmp(imageFileName.c_str(), imgw, imgh, imgch, m_imageBuffer);
		ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
		auto textureId = ObjectLibrary::getInstance().assetPathToId(imageFileName);

		// Construct the struct
		AssetData::SpriteSheetData data(textureId);

		// Copy the glyph data
		data.sprites.reserve(m_sprites.size());
		for (const auto& s : m_sprites)
		{
			data.sprites.push_back(s.second);
		}

		Files::Utils::WriteToDisk(data, filePath);
	}

}
