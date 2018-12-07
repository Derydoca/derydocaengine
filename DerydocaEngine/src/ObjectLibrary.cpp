#include "ObjectLibrary.h"
#include <iostream>
#include <string>
#include <boost\filesystem.hpp>
#include <boost\uuid\uuid_generators.hpp>
#include "Files\Serializers\FileSerializerLibrary.h"
#include "Helpers\StringUtils.h"
#include "yaml-cpp\yaml.h"
#include "Helpers\YamlTools.h"
#include "Files\FileType.h"

namespace DerydocaEngine
{

	namespace fs = boost::filesystem;

	void ObjectLibrary::initialize(std::string const& engineResourcesPath, std::string const& projectPath)
	{
		std::cout << "Updating meta files for the project: " << projectPath << "\n";
		updateMetaFilesDirectory(projectPath);
		std::cout << "Loading project files: " << engineResourcesPath << "\n";
		loadDirectory(projectPath);

		std::cout << "Loading engine files: " << engineResourcesPath << "\n";
		loadDirectory(engineResourcesPath);
	}

	std::shared_ptr<Resources::Resource> ObjectLibrary::getResource(std::string const& uuidString)
	{
		boost::uuids::string_generator gen;
		boost::uuids::uuid uuid = gen(uuidString);
		return getResource(uuid);
	}

	std::shared_ptr<Resources::Resource> ObjectLibrary::getResource(boost::uuids::uuid const& uuid)
	{
		// Find a resource with a matching uuid
		auto search = m_resources.find(uuid);
		if (search != m_resources.end())
		{
			// If we found it, return the reference to the resource
			return search->second;
		}
		else
		{
			// If it was not in the collection, return null
			return nullptr;
		}
	}

	std::shared_ptr<Components::GameComponent> ObjectLibrary::getComponent(boost::uuids::uuid const& id)
	{
		auto search = m_sceneComponents.find(id);
		if (search != m_sceneComponents.end())
		{
			return search->second;
		}

		return nullptr;
	}

	std::shared_ptr<Resources::Resource> ObjectLibrary::getMetaFile(std::string const& sourceFilePath)
	{
		std::string metaFilePath = sourceFilePath + m_metaExtension;

		// Load the meta file
		YAML::Node file = YAML::LoadFile(metaFilePath);
		YAML::Node resourcesNode = file["Resources"];
		if (!resourcesNode)
		{
			std::cout << "The meta file '" << metaFilePath.c_str() << "' does not have a resource node assigned it it. This file could not be parsed!\n";
			return nullptr;
		}

		// Go through all the resource nodes in the file
		for (size_t i = 0; i < resourcesNode.size(); i++)
		{
			YAML::Node resourceNode = resourcesNode[i];

			// If no ID node is defined, exit out because it is required
			if (!resourceNode["ID"])
			{
				std::cout << "A node was skipped because it was missing an ID parameter.\n";
				continue;
			}

			// Get the ID
			boost::uuids::uuid resourceUuid = resourceNode["ID"].as<boost::uuids::uuid>();

			// Find the serializer related to this source file object
			auto serializer = Files::Serializers::FileSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);

			// If the serializer could not be found, continue onto the next resource
			if (serializer == nullptr)
			{
				std::cout << "The file '" << sourceFilePath.c_str() << "' does not have a parser assigned to the extension. This file could not be parsed!\n";
				continue;
			}

			// Load the resource object
			std::shared_ptr<Resources::Resource> resource = serializer->loadResourceFromMeta(resourceNode);

			// If no resource could be parsed from the node, continue on
			if (resource == nullptr)
			{
				std::cout << "Unable to convert node " << i << " to resource. This resource could not be parsed!\n";
				continue;
			}

			// Set the common parameters of the resource object
			resource->setFilePaths(sourceFilePath, metaFilePath);
			resource->setId(resourceUuid);
			serializer->postLoadInitialize(resource);

			return resource;
		}

		return nullptr;
	}

	void ObjectLibrary::updateMetaFilesDirectory(std::string const& directory)
	{
		fs::directory_iterator it{ directory };
		while (it != fs::directory_iterator{})
		{
			if (is_directory(it->path()))
			{
				updateMetaFilesDirectory(it->path().string());
			}
			else
			{
				if (!endsWith(it->path().string(), m_metaExtension))
				{
					updateMetaFiles(it->path().string());
				}
			}
			it++;
		}
	}

	void ObjectLibrary::updateMetaFiles(std::string const& sourceFilePath)
	{
		std::string metaFilePath = sourceFilePath + m_metaExtension;

		// If the meta file does not exist
		if (!fs::exists(metaFilePath))
		{
			// Create the meta file
			if (!createMetaFile(sourceFilePath, metaFilePath))
			{
				// If the meta file was not be created, skip this file
				return;
			}
		}
	}

	void ObjectLibrary::registerComponent(boost::uuids::uuid const& id, std::shared_ptr<Components::GameComponent> component)
	{
		m_sceneComponents.insert(std::pair<boost::uuids::uuid, std::shared_ptr<Components::GameComponent>>(id, component));
	}

	void ObjectLibrary::loadDirectory(std::string const& directory)
	{
		fs::directory_iterator it{ directory };
		while (it != fs::directory_iterator{})
		{
			if (is_directory(it->path()))
			{
				loadDirectory(it->path().string());
			}
			else
			{
				if (!endsWith(it->path().string(), m_metaExtension))
				{
					loadFile(it->path().string());
				}
			}
			it++;
		}
	}

	bool ObjectLibrary::createMetaFile(std::string const& sourceFilePath, std::string const& metaFilePath)
	{
		Files::FileType fileType = Files::pathToFileType(sourceFilePath);

		// If this is a file type we are expected to ignore, lets ignore it
		if (fileType == Files::FileType::IgnoredFileType)
		{
			return false;
		}

		// Find the serializer for this file type
		auto serializer = Files::Serializers::FileSerializerLibrary::getInstance().getTypeSerializer(fileType);

		// If the serializer was not found, abort and return false
		if (serializer == nullptr)
		{
			std::cout << "The file '" << sourceFilePath.c_str() << "' does not have a serializer associated with it.\n";
			return false;
		}

		// Create the yaml structure
		YAML::Node root;
		YAML::Node resources = serializer->generateResourceNodes(sourceFilePath);
		root["Resources"] = resources;

		// Create the file emitter and save it to disk
		YAML::Emitter out;
		out.SetIndent(2);
		out.SetMapFormat(YAML::Block);
		out << root;
		std::ofstream file;
		file.open(metaFilePath);
		file << out.c_str();
		file.close();

		return true;
	}

	void ObjectLibrary::registerResource(std::shared_ptr<Resources::Resource> resource)
	{
		// Load the resource into the map
		m_resources.insert(std::pair<boost::uuids::uuid, std::shared_ptr<Resources::Resource>>(resource->getId(), resource));
	}

	void ObjectLibrary::loadFile(std::string const& sourceFilePath)
	{
		std::string metaFilePath = sourceFilePath + m_metaExtension;

		// If the meta file does not exist, skip loading this resource
		if (!fs::exists(metaFilePath))
		{
			return;
		}

		// Load the meta file
		YAML::Node file = YAML::LoadFile(metaFilePath);
		YAML::Node resourcesNode = file["Resources"];
		if (!resourcesNode)
		{
			std::cout << "The meta file '" << metaFilePath.c_str() << "' does not have a resource node assigned it it. This file could not be parsed!\n";
		}

		// Go through all the resource nodes in the file
		for (size_t i = 0; i < resourcesNode.size(); i++)
		{
			YAML::Node resourceNode = resourcesNode[i];

			// If no ID node is defined, exit out because it is required
			if (!resourceNode["ID"])
			{
				std::cout << "A node was skipped because it was missing an ID parameter.\n";
				continue;
			}

			// Get the ID
			boost::uuids::uuid resourceUuid = resourceNode["ID"].as<boost::uuids::uuid>();

			// Find the serializer related to this source file object
			auto serializer = Files::Serializers::FileSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);

			// If the serializer could not be found, continue onto the next resource
			if (serializer == nullptr)
			{
				std::cout << "The file '" << sourceFilePath.c_str() << "' does not have a parser assigned to the extension. This file could not be parsed!\n";
				continue;
			}

			// Load the resource object
			std::shared_ptr<Resources::Resource> resource = serializer->loadResourceFromMeta(resourceNode);

			// If no resource could be parsed from the node, continue on
			if (resource == nullptr)
			{
				std::cout << "Unable to convert node " << i << " to resource. This resource could not be parsed!\n";
				continue;
			}

			// Set the common parameters of the resource object
			resource->setFilePaths(sourceFilePath, metaFilePath);
			resource->setId(resourceUuid);
			serializer->postLoadInitialize(resource);

			// Register the resource so it can be referenced later
			registerResource(resource);
		}

	}

}
