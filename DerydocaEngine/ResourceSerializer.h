#pragma once
#include <iostream>
#include "ResourceType.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include "ObjectLibrary.h"
#include "Resource.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ResourceSerializer
	{
	public:
		virtual ~ResourceSerializer() {}
		virtual void* deserialize(Resource* const& resource) = 0;
		virtual ResourceType getResourceType() = 0;

	protected:
		template<typename T>
		inline T loadResource(YAML::Node const& node, std::string const& resourceName)
		{
			YAML::Node resourceIdNode = node[resourceName];
			if (!resourceIdNode)
			{
				std::cout << "Unable to load resource because the ID node of '" << resourceName << "' could not be found.\n";
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

		std::string getSourceFilePath(YAML::Node const& node, std::string const& resourceName)
		{
			boost::uuids::uuid id = node[resourceName].as<boost::uuids::uuid>();
			Resource * resource = ObjectLibrary::getInstance().getResource(id);
			if (!resource)
			{
				std::cout << "Unable to get the source file path of '" << resourceName << "' from the asset with ID of '" << boost::uuids::to_string(id) << "'.\n";
				return "";
			}
			return resource->getSourceFilePath();
		}
	};

}
