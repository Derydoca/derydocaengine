#pragma once
#include "TexturePackerImage.h"
#include <string>

TexturePackerImage::TexturePackerImage(
	unsigned long const& id,
	int const& width, 
	int const& height, 
	int const& channels, 
	float const& sizeX, 
	float const& sizeY, 
	float const& bearingX, 
	float const& bearingY, 
	float const& advanceX, 
	float const& advanceY)
{
	m_id = id;
	m_width = width;
	m_height = height;
	m_channels = channels;
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

void TexturePackerImage::setTextureSheetRectangle(float const& x, float const& y, float const& dx, float const& dy)
{
	m_texSheetPosition.setX(x);
	m_texSheetPosition.setY(y);
	m_texSheetPosition.setDX(dx);
	m_texSheetPosition.setDY(dy);
}
