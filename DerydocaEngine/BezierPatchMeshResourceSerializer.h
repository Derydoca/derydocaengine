#pragma once
#include "MaterialResourceSerializer.h"
class BezierPatchMeshResourceSerializer : public MaterialResourceSerializer
{
public:
	BezierPatchMeshResourceSerializer();
	~BezierPatchMeshResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};

