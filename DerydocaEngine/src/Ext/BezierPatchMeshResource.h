#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct BezierPatchMeshResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(BezierPatchMeshResource);

		BezierPatchMeshResource()
		{
			setType(DerydocaEngine::Resources::BezierPatchMeshResourceType);
		}

	};

}
