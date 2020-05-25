#include "EnginePch.h"
#include "UI\Spritesheet.h"
#include "ObjectLibrary.h"
#include "Helpers\YamlTools.h"
#include <yaml-cpp\yaml.h>
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

	void SpriteSheet::saveToDisk(const std::string& filePath)
	{
		//auto textureId = ObjectLibrary::getInstance().assetPathToId(imageFileName);
		//AssetData::SpriteSheetData data("Engine Sprites", );
		
		//TODO: Replace YAML
		//// Save the image to disk and process it by the object library
		//std::string imageFileName = filePath + ".tga";
		//stbi_write_tga(imageFileName.c_str(), m_texture->getWidth(), m_texture->getHeight(), 4, m_imageBuffer);
		//ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
		//auto imageResource = ObjectLibrary::getInstance().getMetaFile(imageFileName);

		//// Create the root and add all root level data
		//YAML::Node root = YAML::Node();
		//YAML::Node spriteSheetNode = root["SpriteSheet"];
		//spriteSheetNode["Texture"] = boost::lexical_cast<std::string>(imageResource->getId());

		//// Add all sprite data to a parent "Sprites" node
		//YAML::Node spritesNode = spriteSheetNode["Sprites"];
		//for (auto sprite : m_sprites)
		//{
		//	// Create the sprite node
		//	YAML::Node spriteNode;
		//	Rect tex = sprite.second.getTexPosition();
		//	spriteNode["Id"] = sprite.second.getId();
		//	spriteNode["Texture"] = sprite.second.getTextureId();
		//	spriteNode["Width"] = sprite.second.getWidth();
		//	spriteNode["Height"] = sprite.second.getHeight();
		//	spriteNode["TexX"] = tex.getX();
		//	spriteNode["TexY"] = tex.getY();
		//	spriteNode["TexDX"] = tex.getDX();
		//	spriteNode["TexDY"] = tex.getDY();
		//	spriteNode["Type"] = (int)sprite.second.getType();
		//	spriteNode["SliceTop"] = sprite.second.getSliceTop();
		//	spriteNode["SliceRight"] = sprite.second.getSliceRight();
		//	spriteNode["SliceBottom"] = sprite.second.getSliceBottom();
		//	spriteNode["SliceLeft"] = sprite.second.getSliceLeft();

		//	// Add this sprite to the parent node
		//	spritesNode.push_back(spriteNode);
		//}

		//// Write this to disk
		//YAML::Emitter out;
		//out.SetIndent(2);
		//out.SetMapFormat(YAML::Block);
		//out << root;
		//std::ofstream file;
		//file.open(filePath);
		//file << out.c_str();
		//file.close();
	}

	void SpriteSheet::LoadFromDisk(std::string const& filePath)
	{
		YAML::Node file = YAML::LoadFile(filePath);
		YAML::Node spriteSheetNode = file["SpriteSheet"];

		bool hasBeenProcessed = spriteSheetNode["Texture"];

		if (hasBeenProcessed)
		{
			std::string textureUuid = spriteSheetNode["Texture"].as<std::string>();
			auto r = ObjectLibrary::getInstance().getResource(textureUuid);
			int imgw, imgh, imgch;
			m_imageBuffer = stbi_load(r->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
			m_texture->updateBuffer(m_imageBuffer, imgw, imgh, imgch, nullptr);
		}

		// Add all sprite data to a parent "Sprites" node
		int nextId = 0;
		YAML::Node spritesNode = spriteSheetNode["Sprites"];
		for (size_t i = 0; i < spritesNode.size(); i++)
		{
			YAML::Node spriteNode = spritesNode[i];

			int spriteId = hasBeenProcessed ? spriteNode["Id"].as<int>() : ++nextId;

			SpriteReference sprite = SpriteReference(spriteId);
			sprite.textureId = spriteNode["Texture"].as<std::string>();
			if (hasBeenProcessed)
			{
				sprite.size = { spriteNode["Width"].as<int>(), spriteNode["Height"].as<int>() };
				sprite.texPosition.set(
					spriteNode["TexX"].as<float>(),
					spriteNode["TexY"].as<float>(),
					spriteNode["TexDX"].as<float>(),
					spriteNode["TexDY"].as<float>()
				);
				int typeId = spriteNode["Type"].as<int>();
				sprite.type = (SpriteType)typeId;
				sprite.slice = {
					spriteNode["SliceTop"].as<float>(),
					spriteNode["SliceRight"].as<float>(),
					spriteNode["SliceBottom"].as<float>(),
					spriteNode["SliceLeft"].as<float>()
				};
			}

			// Add this sprite to the parent node
			m_sprites[spriteId] = sprite;
		}

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
		int imgw, imgh, imgch;
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
