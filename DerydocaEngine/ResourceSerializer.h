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

protected:
	template<typename T>
	inline T loadResource(YAML::Node node, std::string resourceName)
	{
		boost::uuids::uuid id = node[resourceName].as<boost::uuids::uuid>();
		Resource * resource = ObjectLibrary::getInstance().getResource(id);
		if (resource)
		{
			return (T)resource->getResourceObject();
		}
		else
		{
			return nullptr;
		}
	}
};

