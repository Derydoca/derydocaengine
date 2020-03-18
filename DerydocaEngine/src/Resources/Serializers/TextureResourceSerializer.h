#pragma once
#include "Resources\Serializers\MaterialResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class TextureResourceSerializer : public MaterialResourceSerializer
	{
	public:
		virtual std::shared_ptr<Object> deserializePointer(std::shared_ptr<Resource> resource);
		virtual ResourceType getResourceType() { return ResourceType::TextureResourceType; }
	};

}
