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
		SpriteSheetData(const boost::uuids::uuid& textureId) :
			textureId(textureId),
			sprites()
		{
		}

		boost::uuids::uuid textureId;
		std::vector<UI::SpriteReference> sprites;

		SERIALIZE_FUNC(
			SERIALIZE(textureId),
			SERIALIZE(sprites)
		);
	};
}