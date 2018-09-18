#pragma once
#include "MaterialResourceSerializer.h"

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshResourceSerializer : public Resources::Serializers::MaterialResourceSerializer
	{
	public:
		BezierPatchMeshResourceSerializer() {}
		~BezierPatchMeshResourceSerializer() {}

		void* deserialize(Resource* const& resource);
		ResourceType getResourceType();
	};

}
