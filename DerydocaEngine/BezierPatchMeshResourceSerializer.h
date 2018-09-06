#pragma once
#include "MaterialResourceSerializer.h"
class BezierPatchMeshResourceSerializer : public MaterialResourceSerializer
{
public:
	BezierPatchMeshResourceSerializer();
	~BezierPatchMeshResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};

