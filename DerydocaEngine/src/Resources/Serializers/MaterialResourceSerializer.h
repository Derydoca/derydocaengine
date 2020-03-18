#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class MaterialResourceSerializer : public ResourceSerializer
	{
	public:
		virtual std::shared_ptr<Object> deserializePointer(std::shared_ptr<Resource> resource);
		virtual ResourceType getResourceType() { return ResourceType::MaterialResourceType; }
	};

}
