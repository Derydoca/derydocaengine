#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct RasterFontResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(RasterFontResource);

		RasterFontResource() :
			Resource(ResourceType::RasterFontResourceType)
		{}

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::RasterFontResource);
