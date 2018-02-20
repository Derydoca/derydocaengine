#pragma once
#include "ResourceType.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include "Resource.h"
#include "ObjectLibrary.h"
#include <iostream>

using namespace std;

class ResourceSerializer
{
public:
	virtual void* deserialize(Resource* resource) = 0;
	virtual ResourceType getResourceType() = 0;

protected:
	template<typename T>
	inline T loadResource(YAML::Node node, std::string resourceName)
	{
		YAML::Node resourceIdNode = node[resourceName];
		if (!resourceIdNode)
		{
			cout << "Unable to load resource because the ID node of '" << resourceName << "' could not be found." << endl;
			return nullptr;
		}
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

	std::string getSourceFilePath(YAML::Node node, std::string resourceName)
	{
		boost::uuids::uuid id = node[resourceName].as<boost::uuids::uuid>();
		Resource * resource = ObjectLibrary::getInstance().getResource(id);
		if(!resource)
		{
			cout << "Unable to get the source file path of '" << resourceName << "' from the asset with ID of '" << boost::uuids::to_string(id) << "'." << endl;
			return "";
		}
		return resource->getSourceFilePath();
	}
};

