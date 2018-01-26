#pragma once
#include <string>
#include "Resource.h"
#include "ResourceType.h"

class ResourceTypeSerializer
{
public:
	virtual ResourceType getResourceType() = 0;
	virtual void* deserialize(Resource* resource) = 0;
};