#pragma once
#include <glm/glm.hpp>
#include "Rect.h"

namespace DerydocaEngine::Utilities
{

	struct TexturePackerImage
	{
		TexturePackerImage() {};
		TexturePackerImage(
			const unsigned long id,
			const int2& imageSize,
			const int channels,
			const float2& worldSize,
			const float2& bearing,
			const float2& advance);
		~TexturePackerImage();

		void setTextureSheetRectangle(const float x, const float y, const float dx, const float dy);

		SERIALIZE_FUNC(
			SERIALIZE(id),
			SERIALIZE(imageSize),
			SERIALIZE(channels),
			SERIALIZE(worldSize),
			SERIALIZE(advance),
			SERIALIZE(bearing),
			SERIALIZE(texSheetPosition)
		);

		unsigned long id;
		int2 imageSize;
		int channels;
		float2 worldSize;
		float2 advance;
		float2 bearing;
		Rect texSheetPosition;
	};

}
