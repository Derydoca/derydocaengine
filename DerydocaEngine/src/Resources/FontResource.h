#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct FontResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(FontResource);

		FontResource() :
			Resource(ResourceType::FontResourceType)
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

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Resources::FontResource);
