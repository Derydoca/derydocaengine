#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct MaterialResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(MaterialResource);

		MaterialResource() :
			Resource(ResourceType::MaterialResourceType)
		{}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource)
			);
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(
				SERIALIZE_BASE(DerydocaEngine::Resources::Resource)
			);
		}

	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::MaterialResource);
