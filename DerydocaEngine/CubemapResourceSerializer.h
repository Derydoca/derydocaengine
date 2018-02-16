#pragma once
#include "ResourceSerializer.h"

class CubemapResourceSerializer : public ResourceSerializer
{
public:
	CubemapResourceSerializer();
	~CubemapResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};