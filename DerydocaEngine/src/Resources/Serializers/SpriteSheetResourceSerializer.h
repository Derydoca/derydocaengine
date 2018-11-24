#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class SpriteSheetResourceSerializer : public ResourceSerializer
	{
	public:
		SpriteSheetResourceSerializer();
		~SpriteSheetResourceSerializer();

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
