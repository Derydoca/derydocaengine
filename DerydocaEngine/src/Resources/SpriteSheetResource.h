#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct SpriteSheetResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(SpriteSheetResource);

		SpriteSheetResource()
		{
			setType(DerydocaEngine::Resources::SpriteSheetType);
		}

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);

	};

}
