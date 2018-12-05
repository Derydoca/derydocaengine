#pragma once
#include "Resources\Serializers\MaterialResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class TextureResourceSerializer : public MaterialResourceSerializer
	{
	public:
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		virtual ResourceType getResourceType() { return ResourceType::TextureResourceType; }
	};

}
