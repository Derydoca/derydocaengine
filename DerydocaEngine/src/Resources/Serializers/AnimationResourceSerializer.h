#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class AnimationResourceSerializer : public ResourceSerializer
	{
	public:
		AnimationResourceSerializer();
		~AnimationResourceSerializer();

		void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		ResourceType getResourceType();
	};

}
