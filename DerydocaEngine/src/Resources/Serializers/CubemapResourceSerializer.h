#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class CubemapResourceSerializer : public ResourceSerializer
	{
	public:
		CubemapResourceSerializer();
		~CubemapResourceSerializer();

		virtual void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		ResourceType getResourceType();
	};

}
