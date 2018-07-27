#include "FontFace.h"
#include <iostream>
#include "TexturePacker.h"
#include "Texture.h"

FontFace::FontFace()
{
	FT_Error error = FT_Init_FreeType(&m_library);
	if (error)
	{
		cout << "An error occurred while attempting to initialize FreeType. Error: " << error << endl;
	}

	error = FT_New_Face(m_library, m_fontFilePath.c_str(), 0, &m_face);
	if (error == FT_Err_Unknown_File_Format)
	{
		cout << "Unable to load the font because it is of an unknown file format. File: " << m_fontFilePath << endl;
		return;
	}
	else if (error)
	{
		cout << "Unable to load the font. Error: " << error << endl;
		return;
	}

	error = FT_Set_Char_Size(m_face, 0, (int)(m_fontSize * 64.0f), m_dotsPerInch.x, m_dotsPerInch.y);

	TexturePacker packer = TexturePacker();

	for (int i = 32; i < 176; i++)
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

	packer.packImages();
	m_texture = packer.allocTexture();
	vector<TexturePackerImage> images = packer.getSubImageData();
	m_charImages.clear();
	for (auto image : images)
	{
		m_charImages[image.getID()] = image;
	}
}

FontFace::~FontFace()
{
}
