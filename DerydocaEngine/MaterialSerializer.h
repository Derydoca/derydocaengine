#pragma once
#include "ResourceTypeSerializer.h"

class MaterialSerializer : public ResourceTypeSerializer
{
public:
	MaterialSerializer();
	~MaterialSerializer();

	virtual void* deserialize(Resource* resource);
	YAML::Node generateResourceNodes(std::string filePath);
	virtual ResourceType getResourceType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};

