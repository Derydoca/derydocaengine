#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct LevelResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(LevelResource);

		LevelResource()
		{
			setType(DerydocaEngine::Resources::LevelResourceType);
		}

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::LevelResource);
