#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class MaterialResourceSerializer : public ResourceSerializer
	{
	public:
		MaterialResourceSerializer();
		~MaterialResourceSerializer();

		virtual void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		ResourceType getResourceType();
	};

}
