#include "ObjectLibrary.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "FileSerializerLibrary.h"
#include "StringUtils.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"

namespace fs = std::experimental::filesystem::v1;

void ObjectLibrary::initialize(std::string projectPath)
{
	initializeDirectory(projectPath);
}

Resource * ObjectLibrary::getResource(boost::uuids::uuid uuid)
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

GameComponent * ObjectLibrary::getComponent(boost::uuids::uuid id)
{
	auto search = m_sceneComponents.find(id);
	if (search != m_sceneComponents.end())
	{
		return search->second;
	}

	return nullptr;
}

void ObjectLibrary::registerComponent(boost::uuids::uuid id, GameComponent * component)
{
	m_sceneComponents.insert(std::pair<uuid, GameComponent*>(id, component));
}

void ObjectLibrary::initializeDirectory(std::string directory)
{
	printf("Loading project directory...\n");

	// Iterate through all files in this directory
	for (auto & p : fs::directory_iterator(directory))
	{
		std::string filePath = p.path().string();

		// If the file is not a meta file, initialize it
		if (!endsWith(filePath, m_metaExtension))
		{
			initializeFile(filePath);
		}
	}
}

bool ObjectLibrary::createMetaFile(std::string sourceFilePath, std::string metaFilePath)
{
	// Find the serializer for this file type
	auto serializer = FileSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);

	// If the serializer was not found, abort and return false
	if (serializer == nullptr)
	{
		printf("The file '%s' does not have a serializer associated with it.\n", sourceFilePath.c_str());
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

void ObjectLibrary::registerResource(Resource* resource)
{
	// Load the resource into the map
	m_resources.insert(std::pair<boost::uuids::uuid, Resource*>(resource->getId(), resource));
}

void ObjectLibrary::initializeFile(std::string sourceFilePath)
{
	std::string metaFilePath = sourceFilePath + m_metaExtension;

	// If the meta file does not exist
	if (!fs::exists(metaFilePath))
	{
		// Create the meta file
		if (!createMetaFile(sourceFilePath, metaFilePath))
		{
			// If the meta file could not be created, skip this file
			printf("The file '%s' was not loaded into the object library because the meta file was unable to be created.\n", sourceFilePath.c_str());
			return;
		}
	}

	// Load the meta file
	YAML::Node file = YAML::LoadFile(metaFilePath);
	YAML::Node resourcesNode = file["Resources"];
	if (!resourcesNode)
	{
		printf("The meta file '%s' does not have a resource node assigned it it. This file could not be parsed!\n", metaFilePath.c_str());
	}

	// Go through all the resource nodes in the file
	for (int i = 0; i < resourcesNode.size(); i++)
	{
		YAML::Node resourceNode = resourcesNode[i];

		// If no ID node is defined, exit out because it is required
		if (!resourceNode["ID"])
		{
			printf("A node was skipped because it was missing an ID parameter.\n");
			continue;
		}

		// Get the ID
		boost::uuids::uuid resourceUuid = resourceNode["ID"].as<boost::uuids::uuid>();

		// Find the serializer related to this source file object
		auto serializer = FileSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);

		// If the serializer could not be found, continue onto the next resource
		if (serializer == nullptr)
		{
			printf("The file '%s' does not have a parser assigned to the extension. This file could not be parsed!\n", sourceFilePath.c_str());
			continue;
		}

		// Load the resource object
		Resource* resource = serializer->loadResourceFromMeta(resourceNode);

		// If no resource could be parsed from the node, continue on
		if (resource == nullptr)
		{
			printf("Unable to convert node %i to resource. This resource could not be parsed!\n", i);
			continue;
		}

		// Set the common parameters of the resource object
		resource->setFilePaths(sourceFilePath, metaFilePath);
		resource->setId(resourceUuid);

		// Register the resource so it can be referenced later
		registerResource(resource);
	}

}
