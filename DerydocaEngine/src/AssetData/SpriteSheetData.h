#pragma once
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <string>
#include "UI\SpriteReference.h"
#include "Utilities\TexturePackerImage.h"
#include "Serialization\Serialization.h"

namespace DerydocaEngine::AssetData
{
	struct SpriteSheetData
	{
		SpriteSheetData() {}
		SpriteSheetData(
			const std::string& name,
			const boost::uuids::uuid& textureId) :
			name(name),
			textureId(textureId),
			glyphs()
		{
		}

		std::string name;
		boost::uuids::uuid textureId;
		std::vector<Utilities::TexturePackerImage> glyphs;

		SERIALIZE_FUNC(
			SERIALIZE(name),
			SERIALIZE(textureId),
			SERIALIZE(glyphs)
		);
	};
}