#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class AnimationResourceSerializer : public ResourceSerializer
	{
	public:
		AnimationResourceSerializer();
		~AnimationResourceSerializer();

		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		virtual ResourceType getResourceType();
	};

}
