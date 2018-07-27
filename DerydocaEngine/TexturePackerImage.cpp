#pragma once
#include "TexturePackerImage.h"
#include <string>

TexturePackerImage::TexturePackerImage(int id, int width, int height, int channels, int bearingX, int bearingY, int advanceX, int advanceY)
{
	m_id = id;
	m_width = width;
	m_height = height;
	//m_channels = channels;
	m_bearingX = bearingX;
	m_bearingY = bearingY;
	m_advanceX = advanceX;
	m_advanceY = advanceY;
	m_texSheetPosition = IntRectangle();
}

TexturePackerImage::~TexturePackerImage()
{
}

void TexturePackerImage::setTextureSheetRectangle(int x, int y, int width, int height)
{
	m_texSheetPosition.setX(x);
	m_texSheetPosition.setY(y);
	m_texSheetPosition.setWidth(width);
	m_texSheetPosition.setHeight(height);
}
