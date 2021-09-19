#pragma once
#include <vector>
#include <string>
#include "UI\SpriteReference.h"
#include "Utilities\TexturePackerImage.h"
#include "Serialization\Serialization.h"

namespace DerydocaEngine::AssetData
{
	struct FontFaceData
	{
		FontFaceData() {}
		FontFaceData(
			const std::string& name,
			const std::string& style,
			const float fontSize,
			const float lineHeight,
			const uuids::uuid& textureId) :
			name(name),
			style(style),
			fontSize(fontSize),
			lineHeight(lineHeight),
			textureId(textureId),
			glyphs()
		{
		}

		std::string name;
		std::string style;
		float fontSize;
		float lineHeight;
		uuids::uuid textureId;
		std::vector<Utilities::TexturePackerImage> glyphs;

		SERIALIZE_FUNC(
			SERIALIZE(name),
			SERIALIZE(style),
			SERIALIZE(fontSize),
			SERIALIZE(lineHeight),
			SERIALIZE(textureId),
			SERIALIZE(glyphs)
		);
	};
}