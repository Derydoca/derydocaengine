#include "FontFace.h"
#include <iostream>
#include "TexturePacker.h"
#include "Texture.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include <iostream>
#include <fstream>
#include "stb_image_write.h"
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

	error = FT_Set_Char_Size(m_face, 0, (int)(m_fontSize * 64.0f), m_dotsPerInch.x, m_dotsPerInch.y);

	TexturePacker packer = TexturePacker();

	//for (int i = 32; i < 176; i++)
	for(int i = 65; i < 122; i++)
	{
		FT_ULong charCode = i;
		FT_UInt glyph_index = FT_Get_Char_Index(m_face, charCode);
		if (glyph_index == 0)
		{
			cout << "Unable to get character index from char code " << charCode << "." << endl;
		}

		error = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			cout << "Unable to load glyph at index " << glyph_index << ". Error: " << error;
		}

		if (m_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				cout << "Unable to render glyph." << endl;
			}
		}

		packer.addImage(i, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, m_face->glyph->bitmap_left, m_face->glyph->bitmap_top, m_face->glyph->advance.x, m_face->glyph->advance.y, m_face->glyph->bitmap.buffer);
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

	m_fontSize = font["fontSize"].as<float>();
	// Load the image

	m_textureDirty = true;
}

void FontFace::saveToSerializedFile(string filePath)
{
	using namespace YAML;
	
	Node root = Node();

	Node font = root["Font"];

	font["width"] = m_imageBufferSize.x;
	font["height"] = m_imageBufferSize.y;
	font["fontSize"] = m_fontSize;
	string imageFileName = filePath + ".bmp";
	stbi_write_bmp(imageFileName.c_str(), m_imageBufferSize.x, m_imageBufferSize.y, 1, m_imageBuffer);
	ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
	Resource* imageResource = ObjectLibrary::getInstance().getMetaFile(imageFileName);
	font["image"] = boost::lexical_cast<string>(imageResource->getId());

	YAML::Emitter out;
	out.SetIndent(2);
	out.SetMapFormat(YAML::Block);
	out << root;
	std::ofstream file;
	file.open(filePath);
	file << out.c_str();
	file.close();
}
