#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct MaterialResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(MaterialResource);

		MaterialResource()
		{
			setType(DerydocaEngine::Resources::MaterialResourceType);
		}

	};

}
