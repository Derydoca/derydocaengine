#include "EnginePch.h"
#include "UI\FontFace.h"
#include "Utilities\TexturePacker.h"
#include "Rendering\Texture.h"
#include "yaml-cpp\yaml.h"
#include "Helpers\YamlTools.h"
#include <fstream>
#include "ObjectLibrary.h"
#include "ObjectLibrary.h"
#include <iostream>
#include "AssetData\FontFaceData.h"
#include "Files\FileUtils.h"

namespace DerydocaEngine::UI
{

	FontFace::FontFace() :
		m_dotsPerInch(300, 300),
		m_fontSize(16.0f),
		m_texture(std::make_shared<Rendering::Texture>()),
		m_charImages(),
		m_imageBuffer(nullptr),
		m_imageBufferSize(0, 0),
		m_textureDirty(false),
		m_lineHeight(50.0f),
		m_name(),
		m_style()
	{
	}

	FontFace::~FontFace()
	{
	}

	std::shared_ptr<Rendering::Texture> FontFace::getTexture()
	{
		// Upload the texture to the GPU if the image buffer has changed and has not been submitted
		if (m_textureDirty)
		{
			m_texture->updateBuffer(m_imageBuffer, m_imageBufferSize.x, m_imageBufferSize.y, 1, nullptr);
			m_textureDirty = false;
		}

		return m_texture;
	}

	void FontFace::loadFromFontFile(std::string const& filePath)
	{
		// Initialize FreeType
		FT_Library freeTypeLibrary;
		FT_Error error = FT_Init_FreeType(&freeTypeLibrary);
		if (error)
		{
			D_LOG_ERROR("An error occurred while attempting to initialize FreeType. Error: {}", error);
		}

		// Load the font file
		FT_Face fontFace;
		error = FT_New_Face(freeTypeLibrary, filePath.c_str(), 0, &fontFace);
		if (error == FT_Err_Unknown_File_Format)
		{
			D_LOG_ERROR("Unable to load the font because it is of an unknown file format. File: {}", filePath);
			return;
		}
		else if (error)
		{
			D_LOG_ERROR("Unable to load the font. Error: {}", error);
			return;
		}

		// Store information about the font
		m_name = fontFace->family_name;
		if (fontFace->style_name != nullptr)
		{
			m_style = fontFace->style_name;
		}
		m_lineHeight = fontFace->height / 64.0f;

		// Set the font size
		error = FT_Set_Char_Size(fontFace, 0, (int)(m_fontSize * 64.0f), m_dotsPerInch.x, m_dotsPerInch.y);

		// Create a texture packer to save all of the glyphs into one texture
		Utilities::TexturePacker packer = Utilities::TexturePacker(1);

		// For every character that exists in the font face
		for (long i = 0; i < fontFace->num_glyphs; i++)
		{
			// Get the character index
			FT_UInt glyph_index = FT_Get_Char_Index(fontFace, i);
			if (glyph_index == 0)
			{
				continue;
			}

			// Get the character glyph data
			error = FT_Load_Glyph(fontFace, glyph_index, FT_LOAD_DEFAULT);
			if (error)
			{
				D_LOG_ERROR("Unable to load glyph at index {}. Error: ", glyph_index, error);
				continue;
			}

			// If the glyph format is not bitmap, then render the glyph so we get a bitmap to store in the texture
			if (fontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
			{
				error = FT_Render_Glyph(fontFace->glyph, FT_RENDER_MODE_NORMAL);
				if (error)
				{
					D_LOG_ERROR("Unable to render glyph.");
					continue;
				}
			}

			// Add the glyph to the texture packer
			packer.addImage(
				i,\
				{
					fontFace->glyph->metrics.horiBearingX / 64.0f,
					fontFace->glyph->metrics.horiBearingY / 64.0f
				},
				{
					fontFace->glyph->metrics.horiAdvance / 64.0f,
					0.0f
				},
				fontFace->glyph->bitmap.buffer,
				{
					static_cast<int>(fontFace->glyph->bitmap.width),
					static_cast<int>(fontFace->glyph->bitmap.rows)
				},
				1);
		}

		// Pack the images
		packer.packImages();

		// Store the image buffer
		m_imageBuffer = packer.allocImageBuffer();
		m_imageBufferSize.x = packer.getWidth();
		m_imageBufferSize.y = packer.getHeight();
		m_textureDirty = true;

		// Load the character image data map
		std::vector<Utilities::TexturePackerImage> images = packer.getSubImageData();
		m_charImages.clear();
		for (auto image : images)
		{
			m_charImages[image.id] = image;
		}
	}

	void FontFace::loadFromSerializedFile(std::string const& filePath)
	{
		YAML::Node file = YAML::LoadFile(filePath);

		YAML::Node font = file["Font"];

		// Load general font information
		m_name = font["name"].as<std::string>();
		m_style = font["style"].as<std::string>();
		m_fontSize = font["fontSize"].as<float>();
		m_imageBufferSize.x = font["width"].as<int>();
		m_imageBufferSize.y = font["height"].as<int>();
		m_lineHeight = font["lineHeight"].as<float>();

		// Load the image
		std::string imageUuid = font["image"].as<std::string>();
		auto r = ObjectLibrary::getInstance().getResource(imageUuid);
		int imgw, imgh, imgch;
		unsigned char* imageData = stbi_load(r->getSourceFilePath().c_str(), &imgw, &imgh, &imgch, 0);
		delete[] m_imageBuffer;
		m_imageBuffer = new unsigned char[m_imageBufferSize.x * m_imageBufferSize.y];
		for (int i = 0; i < m_imageBufferSize.x * m_imageBufferSize.y; i++)
		{
			m_imageBuffer[i] = imageData[i * imgch];
		}
		delete[] imageData;

		// Load all character data
		YAML::Node charactersNode = font["characters"];
		for (size_t i = 0; i < charactersNode.size(); i++)
		{
			YAML::Node charNode = charactersNode[i];
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
			Utilities::TexturePackerImage img(id, 1, { w, h }, { bx, by }, { ax, ay });
			img.imageRect = { tx, ty, tdx, tdy };
			m_charImages.emplace(id, img);
		}

		m_textureDirty = true;
	}

	void FontFace::saveToSerializedFile(std::string const& filePath)
	{
		// Write the generated texture to disk
		std::string imageFileName = filePath + ".bmp";
		stbi_write_bmp(imageFileName.c_str(), m_imageBufferSize.x, m_imageBufferSize.y, 1, m_imageBuffer);
		ObjectLibrary::getInstance().updateMetaFiles(imageFileName);
		auto textureId = ObjectLibrary::getInstance().assetPathToId(imageFileName);

		// Construct the struct
		AssetData::FontFaceData data(
			m_name,
			m_style,
			m_fontSize,
			m_lineHeight,
			boost::uuids::uuid()
		);
		data.textureId = textureId;

		// Copy the glyph data
		data.glyphs.reserve(m_charImages.size());
		for (const auto& s : m_charImages)
		{
			data.glyphs.push_back(s.second);
		}
		
		Files::Utils::WriteToDisk(data, filePath);
	}

}
