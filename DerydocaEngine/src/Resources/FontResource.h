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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::FontResource);
