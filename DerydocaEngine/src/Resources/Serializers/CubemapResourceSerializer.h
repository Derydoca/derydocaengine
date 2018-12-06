#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class CubemapResourceSerializer : public ResourceSerializer
	{
	public:
		virtual std::shared_ptr<void> deserializePointer(std::shared_ptr<Resource> resource);
		virtual ResourceType getResourceType() { return ResourceType::CubemapResourceType; }
	};

}
