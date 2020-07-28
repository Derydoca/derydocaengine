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
			const int channels,
			const int2& size,
			const float2& bearing,
			const float2& advance) :
			id(id),
			size(size),
			channels(channels),
			bearing(bearing),
			advance(advance),
			imageRect()
		{
		}
		~TexturePackerImage() {};

		SERIALIZE_FUNC(
			SERIALIZE(id),
			SERIALIZE(channels),
			SERIALIZE(size),
			SERIALIZE(advance),
			SERIALIZE(bearing),
			SERIALIZE(imageRect)
		);

		unsigned long id;
		int channels;
		int2 size;
		float2 advance;
		float2 bearing;
		Rect imageRect;
	};

}
