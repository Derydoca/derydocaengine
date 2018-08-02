#pragma once
#include "TexturePackerImage.h"
#include <string>

TexturePackerImage::TexturePackerImage(unsigned long id, int width, int height, int channels, float sizeX, float sizeY, float bearingX, float bearingY, float advanceX, float advanceY)
{
	m_id = id;
	m_width = width;
	m_height = height;
	//m_channels = channels;
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_bearingX = bearingX;
	m_bearingY = bearingY;
	m_advanceX = advanceX;
	m_advanceY = advanceY;
	m_texSheetPosition = Rect();
}

TexturePackerImage::~TexturePackerImage()
{
}

void TexturePackerImage::setTextureSheetRectangle(float x, float y, float dx, float dy)
{
	m_texSheetPosition.setX(x);
	m_texSheetPosition.setY(y);
	m_texSheetPosition.setDX(dx);
	m_texSheetPosition.setDY(dy);
}
