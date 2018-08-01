#pragma once
#include <glm/glm.hpp>
#include "Rect.h"

using namespace std;

struct TexturePackerImage
{
public:
	TexturePackerImage() {};
	TexturePackerImage(int id, int width, int height, int channels, float sizeX, float sizeY, float bearingX, float bearingY, float advanceX, float advanceY);
	~TexturePackerImage();

	int getID() const { return m_id; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	//int getChannels() const { return m_channels; }
	float getSizeX() const { return m_sizeX; }
	float getSizeY() const { return m_sizeY; }
	float getBearingX() const { return m_bearingX; }
	float getBearingY() const { return m_bearingY; }
	float getAdvanceX() const { return m_advanceX; }
	float getAdvanceY() const { return m_advanceY; }
	Rect getTexSheetPosition() { return m_texSheetPosition; }

	void setTextureSheetRectangle(float x, float y, float width, float height);

private:
	int m_id;
	int m_width;
	int m_height;
	//int m_channels;
	float m_sizeX;
	float m_sizeY;
	float m_advanceX;
	float m_advanceY;
	float m_bearingX;
	float m_bearingY;
	Rect m_texSheetPosition;
};
