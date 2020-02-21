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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Resources::Resource);
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::BezierPatchMeshResource);
