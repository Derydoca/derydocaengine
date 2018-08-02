#include "FontFace.h"
#include <iostream>
#include "TexturePacker.h"
#include "Texture.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include <iostream>
#include <fstream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "ObjectLibrary.h"
#include "ObjectLibrary.h"

FontFace::FontFace()
{
}

FontFace::~FontFace()
{
}

Texture * FontFace::getTexture()
{
	// Upload the texture to the GPU if the image buffer has changed and has not been submitted
	if (m_textureDirty)
	{
		m_texture.updateBuffer(m_imageBuffer, m_imageBufferSize.x, m_imageBufferSize.y, GL_RED, nullptr);
		m_textureDirty = false;
	}

	return &m_texture;
}

void FontFace::loadFromFontFile(string filePath)
{
	FT_Error error = FT_Init_FreeType(&m_library);
	if (error)
	{
		cout << "An error occurred while attempting to initialize FreeType. Error: " << error << endl;
	}

	error = FT_New_Face(m_library, filePath.c_str(), 0, &m_face);
	if (error == FT_Err_Unknown_File_Format)
	{
		cout << "Unable to load the font because it is of an unknown file format. File: " << filePath << endl;
		return;
	}
	else if (error)
	{
		cout << "Unable to load the font. Error: " << error << endl;
		return;
	}

	m_name = m_face->family_name;
	if (m_face->style_name != nullptr)
	{
		m_style = m_face->style_name;
	}
	m_lineHeight = m_face->height / 64.0f;

	error = FT_Set_Char_Size(m_face, 0, (int)(m_fontSize * 64.0f), m_dotsPerInch.x, m_dotsPerInch.y);

	TexturePacker packer = TexturePacker();

	for(unsigned long i = 0; i < m_face->num_glyphs; i++)
	{
		FT_ULong charCode = i;
		FT_UInt glyph_index = FT_Get_Char_Index(m_face, charCode);
		if (glyph_index == 0)
		{
			cout << "Unable to get character index from char code " << charCode << "." << endl;
			continue;
		}

		error = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			cout << "Unable to load glyph at index " << glyph_index << ". Error: " << error;
			continue;
		}

		if (m_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				cout << "Unable to render glyph." << endl;
				continue;
			}
		}

		packer.addImage(
			i,
			m_face->glyph->metrics.width / 64.0f,
			m_face->glyph->metrics.height / 64.0f,
			m_face->glyph->metrics.horiBearingX / 64.0f,
			m_face->glyph->metrics.horiBearingY/ 64.0f,
			m_face->glyph->metrics.horiAdvance / 64.0f,
			0.0f,
			m_face->glyph->bitmap.buffer,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows);
	}

	// Pack the images
	packer.packImages();

	// Store the image buffer
	m_imageBuffer = packer.allocImageBuffer();
	m_imageBufferSize.x = packer.getWidth();
	m_imageBufferSize.y = packer.getHeight();
	m_textureDirty = true;

	// Load the character image data map
	vector<TexturePackerImage> images = packer.getSubImageData();
	m_charImages.clear();
	for (auto image : images)
	{
		m_charImages[image.getID()] = image;
	}
}

void FontFace::loadFromSerializedFile(string filePath)
{
	using namespace YAML;

	Node file = YAML::LoadFile(filePath);

	Node font = file["Font"];

	m_name = font["name"].as<string>();
	m_style = font["style"].as<string>();
	m_fontSize = font["fontSize"].as<float>();
	m_imageBufferSize.x = font["width"].as<int>();
	m_imageBufferSize.y = font["height"].as<int>();
	m_lineHeight = font["lineHeight"].as<float>();
	
	// Load the image
	string imageUuid = font["image"].as<string>();
	Resource* r = ObjectLibrary::getInstance().getResource(imageUuid);
	int imgw, imgh, imgch;
	unsigned char* imageData = stbi_load(r->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
	delete[] m_imageBuffer;
	m_imageBuffer = new unsigned char[m_imageBufferSize.x * m_imageBufferSize.y];
	for (int i = 0; i < m_imageBufferSize.x * m_imageBufferSize.y; i++)
	{
		m_imageBuffer[i] = imageData[i * imgch];
	}
	delete[] imageData;

	Node charactersNode = font["characters"];
	for (int i = 0; i < charactersNode.size(); i++)
	{
		Node charNode = charactersNode[i];
		int id = charNode["id"].as<unsigned long>();
		int w = charNode["width"].as<int>();
		int h = charNode["height"].as<int>();
		float sx = charNode["sizeX"].as<float>();
		float sy = charNode["sizeY"].as<float>();
		float ax = charNode["advanceX"].as<float>();
		float ay = charNode["advanceY"].as<float>();
		float bx = charNode["bearingX"].as<float>();
		float by = charNode["bearingY"].as<float>();
		float tx = charNode["texX"].as<float>();
		float ty = charNode["texY"].as<float>();
		float tdx = charNode["texDX"].as<float>();
		float tdy = charNode["texDY"].as<float>();
		TexturePackerImage img(id, w, h, 1, sx, sy, bx, by, ax, ay);
		img.setTextureSheetRectangle(tx, ty, tdx, tdy);
		m_charImages.emplace(id, img);
	}

	m_textureDirty = true;
}

void FontFace::saveToSerializedFile(string filePath)
{
	using namespace YAML;

	Node root = Node();

	Node font = root["Font"];

	font["name"] = m_name;
	font["style"] = m_style;
	font["fontSize"] = m_fontSize;
	font["width"] = m_imageBufferSize.x;
	font["height"] = m_imageBufferSize.y;
	font["lineHeight"] = m_lineHeight;
	string imageFileName = filePath + ".bmp";
	stbi_write_bmp(imageFileName.c_str(), m_imageBufferSize.x, m_imageBufferSize.y, 1, m_imageBuffer);
	ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
	Resource* imageResource = ObjectLibrary::getInstance().getMetaFile(imageFileName);
	font["image"] = boost::lexical_cast<string>(imageResource->getId());

	Node charactersNode = font["characters"];

	for (auto charImage : m_charImages)
	{
		Node charNode;
		charNode["id"] = charImage.second.getID();
		charNode["width"] = charImage.second.getWidth();
		charNode["height"] = charImage.second.getHeight();
		charNode["sizeX"] = charImage.second.getSizeX();
		charNode["sizeY"] = charImage.second.getSizeY();
		charNode["advanceX"] = charImage.second.getAdvanceX();
		charNode["advanceY"] = charImage.second.getAdvanceY();
		charNode["bearingX"] = charImage.second.getBearingX();
		charNode["bearingY"] = charImage.second.getBearingY();
		Rect texPos = charImage.second.getTexSheetPosition();
		charNode["texX"] = texPos.getX();
		charNode["texY"] = texPos.getY();
		charNode["texDX"] = texPos.getDX();
		charNode["texDY"] = texPos.getDY();
		charactersNode.push_back(charNode);
	}

	YAML::Emitter out;
	out.SetIndent(2);
	out.SetMapFormat(YAML::Block);
	out << root;
	std::ofstream file;
	file.open(filePath);
	file << out.c_str();
	file.close();
}
