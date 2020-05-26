#pragma once
#include "Resources\Serializers\MaterialResourceSerializer.h"

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshResourceSerializer : public Resources::Serializers::MaterialResourceSerializer
	{
	public:
		BezierPatchMeshResourceSerializer() {}
		~BezierPatchMeshResourceSerializer() {}

		virtual std::shared_ptr<void> deserializePointer(std::shared_ptr<Resources::Resource> resource);
		virtual Resources::ResourceType getResourceType();
	};

}
