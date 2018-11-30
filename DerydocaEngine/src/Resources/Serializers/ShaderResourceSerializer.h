#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ShaderResourceSerializer : public ResourceSerializer
	{
	public:
		ShaderResourceSerializer() {}
		~ShaderResourceSerializer() {}

		virtual void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		ResourceType getResourceType();
	};

}
