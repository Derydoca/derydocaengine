#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class MaterialResourceSerializer : public ResourceSerializer
	{
	public:
		MaterialResourceSerializer();
		~MaterialResourceSerializer();

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
