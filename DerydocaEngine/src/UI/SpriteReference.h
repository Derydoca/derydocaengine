#pragma once
#include "Rect.h"

namespace DerydocaEngine::UI
{

	enum class SpriteType
	{
		Sprite = 0,
		NineSlice = 1
	};

	struct SpriteReference
	{
	public:
		SpriteReference()
		{
		}

		SpriteReference(unsigned long const& identifier)
		{
			id = identifier;
		}

		~SpriteReference()
		{
		}

		SERIALIZE_FUNC(
			SERIALIZE(id),
			SERIALIZE(type),
			SERIALIZE(slice),
			SERIALIZE(size),
			SERIALIZE(texPosition),
			SERIALIZE(textureId)
		)

		unsigned int id;
		SpriteType type = SpriteType::Sprite;
		float4 slice = { 0.0f, 1.0f, 1.0f, 0.0f }; // Top, Right, Bottom, Left
		int2 size;
		Rect texPosition;
		std::string textureId;
	};

}
