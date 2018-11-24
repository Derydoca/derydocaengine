#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ShaderResourceSerializer : public ResourceSerializer
	{
	public:
		ShaderResourceSerializer() {}
		~ShaderResourceSerializer() {}

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
