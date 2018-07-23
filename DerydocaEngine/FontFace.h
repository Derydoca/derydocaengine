#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

class FontFace
{
public:
	FontFace();
	~FontFace();
private:
	FT_Library m_library;
};

