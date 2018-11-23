#pragma once
#include "MaterialResourceSerializer.h"

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshResourceSerializer : public Resources::Serializers::MaterialResourceSerializer
	{
	public:
		BezierPatchMeshResourceSerializer() {}
		~BezierPatchMeshResourceSerializer() {}

		void* deserialize(Resources::Resource* const& resource);
		Resources::ResourceType getResourceType();
	};

}
