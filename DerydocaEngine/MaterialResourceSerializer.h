#pragma once
#include "ResourceSerializer.h"

class MaterialResourceSerializer : public ResourceSerializer
{
public:
	MaterialResourceSerializer();
	~MaterialResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};

