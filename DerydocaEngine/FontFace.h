#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include "Rectangle.h"
#include "TexturePackerImage.h"

using namespace std;
using namespace glm;

class Texture;
const int MISSING_CHAR_CODE = 95;

class FontFace
{
public:
	FontFace();
	~FontFace();

	Texture* getTexture() const { return m_texture; }
	TexturePackerImage getCharData(int charCode) const {
		if (m_charImages.find(charCode) == m_charImages.end())
		{
			return m_charImages.at(MISSING_CHAR_CODE);
		}
		else
		{
			return m_charImages.at(charCode);
		}
	};
	void setFontSize(int fontSize) { m_fontSize = fontSize; }

	void loadFromFile(string filePath);
private:
	FT_Library m_library;
	FT_Face m_face;
	ivec2 m_dotsPerInch = ivec2(300, 300);
	float m_fontSize = 16.0;
	Texture* m_texture;
	map<int, TexturePackerImage> m_charImages;
};

