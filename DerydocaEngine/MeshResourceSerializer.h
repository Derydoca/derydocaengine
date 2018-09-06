#pragma once
#include "ResourceSerializer.h"

class MeshResourceSerializer : public ResourceSerializer
{
public:
	MeshResourceSerializer();
	~MeshResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};

