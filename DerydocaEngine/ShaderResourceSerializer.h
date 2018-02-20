#pragma once
#include "ResourceSerializer.h"

class ShaderResourceSerializer : public ResourceSerializer
{
public:
	ShaderResourceSerializer() {}
	~ShaderResourceSerializer() {}

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};