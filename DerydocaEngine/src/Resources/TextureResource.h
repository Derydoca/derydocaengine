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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);

	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::TextureResource);
