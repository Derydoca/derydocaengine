#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct TextureResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(TextureResource);

		TextureResource()
		{
			setType(DerydocaEngine::Resources::TextureResourceType);
		}

	};

}
