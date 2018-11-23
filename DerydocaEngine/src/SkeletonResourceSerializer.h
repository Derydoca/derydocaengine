#pragma once
#include "ResourceSerializer.h"

namespace DerydocaEngine::Resources::Serializers
{

	class SkeletonResourceSerializer : public ResourceSerializer
	{
	public:
		SkeletonResourceSerializer();
		~SkeletonResourceSerializer();

		void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);

		ResourceType getResourceType();
	};

}
