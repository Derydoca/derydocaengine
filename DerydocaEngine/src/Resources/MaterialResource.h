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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);

	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::MaterialResource);
