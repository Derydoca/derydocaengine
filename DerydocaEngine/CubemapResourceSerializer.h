#pragma once
#include "ResourceSerializer.h"

class CubemapResourceSerializer : public ResourceSerializer
{
public:
	CubemapResourceSerializer();
	~CubemapResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};