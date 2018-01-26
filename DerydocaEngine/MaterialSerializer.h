#pragma once
#include "ResourceTypeSerializer.h"

class MaterialSerializer : public ResourceTypeSerializer
{
public:
	MaterialSerializer();
	~MaterialSerializer();

	virtual ResourceType getResourceType();
	virtual void* deserialize(Resource* resource);
};

