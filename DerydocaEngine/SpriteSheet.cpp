#include "SpriteSheet.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "ObjectLibrary.h"
#include "YamlTools.h"
#include <yaml-cpp/yaml.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

SpriteSheet::SpriteSheet()
{
	m_sprites = map<int, SpriteReference>();
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::updateTexture()
{
	TexturePacker packer = TexturePacker(4);
	ObjectLibrary* ObjLib = &ObjectLibrary::getInstance();
	
	for (auto spriteMapItem : m_sprites)
	{
		SpriteReference sprite = spriteMapItem.second;

		Resource* spriteImageResource = ObjLib->getResource(sprite.getTextureId());
		
		int imgw, imgh, imgch;
		unsigned char* spriteImageBuffer = stbi_load(spriteImageResource->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
		sprite.setWidth(imgw);
		sprite.setHeight(imgh);
		sprite.setType(SpriteType::Sprite);
		packer.addImage(sprite.getId(), (float)imgw, (float)imgh, 0, 0, 0, 0, spriteImageBuffer, imgw, imgh, imgch);
		delete[] spriteImageBuffer;

		m_sprites[spriteMapItem.first] = sprite;
	}

	packer.packImages();

	vector<TexturePackerImage> packedImages = packer.getSubImageData();
	for (auto img : packedImages)
	{
		auto spriteMapRecord = m_sprites.find(img.getID());

		if (spriteMapRecord == m_sprites.end())
		{
			continue;
		}

		auto tex = img.getTexSheetPosition();
		(*spriteMapRecord).second.setTexPosition(tex.getX(), tex.getY(), tex.getDX(), tex.getDY());
	}

	delete[] m_imageBuffer;
	m_imageBuffer = packer.allocImageBuffer();
	m_texture.updateBuffer(m_imageBuffer, packer.getWidth(), packer.getHeight(), packer.getChannels(), nullptr);
}

void SpriteSheet::addSprite(string const& textureId)
{
	SpriteReference sprite = SpriteReference(++m_largestId);
	sprite.setTextureId(textureId);
	sprite.setType(SpriteType::Sprite);
	m_sprites[m_largestId] = sprite;
}

void SpriteSheet::saveToDisk(string const& filePath)
{
	using namespace YAML;

	// Save the image to disk and process it by the object library
	string imageFileName = filePath + ".tga";
	stbi_write_tga(imageFileName.c_str(), m_texture.getWidth(), m_texture.getHeight(), 4, m_imageBuffer);
	ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
	Resource* imageResource = ObjectLibrary::getInstance().getMetaFile(imageFileName);

	// Create the root and add all root level data
	Node root = Node();
	Node spriteSheetNode = root["SpriteSheet"];
	spriteSheetNode["Texture"] = boost::lexical_cast<string>(imageResource->getId());

	// Add all sprite data to a parent "Sprites" node
	Node spritesNode = spriteSheetNode["Sprites"];
	for (auto sprite : m_sprites)
	{
		// Create the sprite node
		Node spriteNode;
		Rect tex = sprite.second.getTexPosition();
		spriteNode["Id"] = sprite.second.getId();
		spriteNode["Texture"] = sprite.second.getTextureId();
		spriteNode["Width"] = sprite.second.getWidth();
		spriteNode["Height"] = sprite.second.getHeight();
		spriteNode["TexX"] = tex.getX();
		spriteNode["TexY"] = tex.getY();
		spriteNode["TexDX"] = tex.getDX();
		spriteNode["TexDY"] = tex.getDY();
		spriteNode["Type"] = (int)sprite.second.getType();
		spriteNode["SliceTop"] = sprite.second.getSliceTop();
		spriteNode["SliceRight"] = sprite.second.getSliceRight();
		spriteNode["SliceBottom"] = sprite.second.getSliceBottom();
		spriteNode["SliceLeft"] = sprite.second.getSliceLeft();

		// Add this sprite to the parent node
		spritesNode.push_back(spriteNode);
	}

	// Write this to disk
	YAML::Emitter out;
	out.SetIndent(2);
	out.SetMapFormat(YAML::Block);
	out << root;
	std::ofstream file;
	file.open(filePath);
	file << out.c_str();
	file.close();
}

void SpriteSheet::LoadFromDisk(string const& filePath)
{
	using namespace YAML;

	Node file = YAML::LoadFile(filePath);
	Node spriteSheetNode = file["SpriteSheet"];

	bool hasBeenProcessed = spriteSheetNode["Texture"];

	if (hasBeenProcessed)
	{
		string textureUuid = spriteSheetNode["Texture"].as<string>();
		Resource *r = ObjectLibrary::getInstance().getResource(textureUuid);
		int imgw, imgh, imgch;
		m_imageBuffer = stbi_load(r->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
		m_texture.updateBuffer(m_imageBuffer, imgw, imgh, imgch, nullptr);
	}

	// Add all sprite data to a parent "Sprites" node
	int nextId = 0;
	Node spritesNode = spriteSheetNode["Sprites"];
	for (size_t i = 0; i < spritesNode.size(); i++)
	{
		Node spriteNode = spritesNode[i];

		int spriteId = hasBeenProcessed ? spriteNode["Id"].as<int>() : ++nextId;

		SpriteReference sprite = SpriteReference(spriteId);
		sprite.setTextureId(spriteNode["Texture"].as<string>());
		if (hasBeenProcessed)
		{
			sprite.setWidth(spriteNode["Width"].as<int>());
			sprite.setHeight(spriteNode["Height"].as<int>());
			sprite.setTexPosition(
				spriteNode["TexX"].as<float>(),
				spriteNode["TexY"].as<float>(),
				spriteNode["TexDX"].as<float>(),
				spriteNode["TexDY"].as<float>()
			);
			int typeId = spriteNode["Type"].as<int>();
			sprite.setType((SpriteType)typeId);
			sprite.setSliceTop(spriteNode["SliceTop"].as<float>());
			sprite.setSliceRight(spriteNode["SliceRight"].as<float>());
			sprite.setSliceBottom(spriteNode["SliceBottom"].as<float>());
			sprite.setSliceLeft(spriteNode["SliceLeft"].as<float>());
		}

		// Add this sprite to the parent node
		m_sprites[spriteId] = sprite;
	}

}
