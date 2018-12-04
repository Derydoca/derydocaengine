#pragma once
#include <iostream>
#include <memory>
#include "Resources\ResourceType.h"
#include "yaml-cpp\yaml.h"
#include "Helpers\YamlTools.h"
#include "ObjectLibrary.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources::Serializers
{

	class ResourceSerializer
	{
	public:
		virtual ~ResourceSerializer() {}
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource) { return nullptr; }
		virtual ResourceType getResourceType() = 0;

	protected:
		template<typename T>
		inline std::shared_ptr<T> loadResourcePointer(YAML::Node const& node, std::string const& resourceName)
		{
			YAML::Node resourceIdNode = node[resourceName];
			if (!resourceIdNode)
			{
				std::cout << "Unable to load resource because the ID node of '" << resourceName << "' could not be found.\n";
				return std::make_shared<T>();
			}
			boost::uuids::uuid id = node[resourceName].as<boost::uuids::uuid>();
			Resource * resource = ObjectLibrary::getInstance().getResource(id);
			if (resource)
			{
				return std::static_pointer_cast<T>(resource->getResourceObjectPointer());
			}
			else
			{
				return std::make_shared<T>();
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
