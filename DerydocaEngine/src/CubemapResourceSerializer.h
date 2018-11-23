#pragma once
#include "ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class CubemapResourceSerializer : public ResourceSerializer
	{
	public:
		CubemapResourceSerializer();
		~CubemapResourceSerializer();

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
