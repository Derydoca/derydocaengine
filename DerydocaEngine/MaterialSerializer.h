#pragma once
#include "ResourceTypeSerializer.h"

class MaterialSerializer : public ResourceTypeSerializer
{
public:
	MaterialSerializer();
	~MaterialSerializer();

	virtual ResourceType getResourceType();
	virtual void* deserialize(Resource* resource);
	YAML::Node generateResourceNodes(std::string filePath);
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};

