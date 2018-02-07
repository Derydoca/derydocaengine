#pragma once
#include "ResourceType.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"

struct Resource;

class ResourceSerializer
{
public:
	virtual void* deserialize(Resource* resource) = 0;
	virtual ResourceType getResourceType() = 0;
};

