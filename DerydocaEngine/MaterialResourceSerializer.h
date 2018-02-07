#pragma once
#include "ResourceSerializer.h"

class MaterialResourceSerializer : public ResourceSerializer
{
public:
	MaterialResourceSerializer();
	~MaterialResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};

