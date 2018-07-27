#pragma once
#include "IntRectangle.h"

using namespace std;

struct TexturePackerImage
{
public:
	TexturePackerImage() {};
	TexturePackerImage(int id, int width, int height, int channels, int bearingX, int bearingY, int advanceX, int advanceY);
	~TexturePackerImage();

	int getID() const { return m_id; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	//int getChannels() const { return m_channels; }
	int getBearingX() const { return m_bearingX; }
	int getBearingY() const { return m_bearingY; }
	int getAdvanceX() const { return m_advanceX; }
	int getAdvanceY() const { return m_advanceY; }
	IntRectangle getTexSheetPosition() const { return m_texSheetPosition; }

	void setTextureSheetRectangle(int x, int y, int width, int height);

private:
	int m_id;
	int m_width;
	int m_height;
	//int m_channels;
	int m_advanceX;
	int m_advanceY;
	int m_bearingX;
	int m_bearingY;
	IntRectangle m_texSheetPosition;
};
