#pragma once
#include "ResourceSerializer.h"

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
