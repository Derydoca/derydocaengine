#include "EnginePch.h"
#pragma once
#include "Utilities\TexturePackerImage.h"
#include <string>

namespace DerydocaEngine::Utilities
{

	TexturePackerImage::TexturePackerImage(const unsigned long id, const int2& imageSize, const int channels, const float2& worldSize, const float2& bearing, const float2& advance) :
		id(id),
		imageSize(imageSize),
		channels(channels),
		worldSize(worldSize),
		bearing(bearing),
		advance(advance),
		texSheetPosition()
	{
	}

	TexturePackerImage::~TexturePackerImage()
	{
	}

	void TexturePackerImage::setTextureSheetRectangle(const float x, const float y, const float dx, const float dy)
	{
		texSheetPosition.setX(x);
		texSheetPosition.setY(y);
		texSheetPosition.setDX(dx);
		texSheetPosition.setDY(dy);
	}

}
