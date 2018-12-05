#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class SpriteSheetResourceSerializer : public ResourceSerializer
	{
	public:
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		virtual ResourceType getResourceType() { return ResourceType::SpriteSheetType; }
	};

}
