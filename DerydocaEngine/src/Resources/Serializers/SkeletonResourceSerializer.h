#pragma once
#include "Resources\Serializers\ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class SkeletonResourceSerializer : public ResourceSerializer
	{
	public:
		SkeletonResourceSerializer();
		~SkeletonResourceSerializer();

		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		virtual ResourceType getResourceType();
	};

}
