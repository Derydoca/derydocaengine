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

	};

}
