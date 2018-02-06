#pragma once
#include <string>
#include "Resource.h"
#include "ResourceType.h"
#include "yaml-cpp\yaml.h"
#include <vector>
#include "YamlTools.h"

#include <boost/uuid/uuid.hpp>

class ResourceTypeSerializer
{
public:
	virtual void* deserialize(Resource* resource) = 0;
	virtual YAML::Node generateResourceNodes(std::string filePath) = 0;
	virtual ResourceType getResourceType() = 0;
	virtual Resource* loadResourceFromMeta(YAML::Node resourceNode) = 0;

	friend boost::uuids::uuid generateUuid();
};