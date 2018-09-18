#pragma once
#include "ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class MeshResourceSerializer : public ResourceSerializer
	{
	public:
		MeshResourceSerializer();
		~MeshResourceSerializer();

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
