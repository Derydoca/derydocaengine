#pragma once
#include "ResourceSerializer.h"

class MeshResourceSerializer : public ResourceSerializer
{
public:
	MeshResourceSerializer();
	~MeshResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};

