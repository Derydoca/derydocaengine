#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include "Rectangle.h"
#include "TexturePackerImage.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class Texture;
const int MISSING_CHAR_CODE = 95;

class FontFace
{
public:
	FontFace();
	~FontFace();

	Texture* getTexture();
	TexturePackerImage getCharData(int const& charCode) const {
		if (m_charImages.find(charCode) == m_charImages.end())
		{
			return m_charImages.at(MISSING_CHAR_CODE);
		}
		else
		{
			return m_charImages.at(charCode);
		}
	};
	float getLineHeight() const { return m_lineHeight; }
	void setFontSize(float const& fontSize) { m_fontSize = fontSize; }

	void loadFromFontFile(string const& filePath);
	void loadFromSerializedFile(string const& filePath);
	void saveToSerializedFile(string const& filePath);
private:
	ivec2 m_dotsPerInch = ivec2(300, 300);
	float m_fontSize = 16.0;
	Texture m_texture;
	map<int, TexturePackerImage> m_charImages;
	unsigned char* m_imageBuffer;
	ivec2 m_imageBufferSize;
	bool m_textureDirty = false;
	float m_lineHeight = 50.0f;
	string m_name = "";
	string m_style = "";
};

