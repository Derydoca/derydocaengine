#include "ObjectLibrary.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iostream>
#include <string>
#include "ResourceSerializerLibrary.h"
#include "StringUtils.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"

namespace fs = std::experimental::filesystem::v1;
std::string m_metaExtension = ".derymeta";

void ObjectLibrary::initialize(std::string projectPath)
{
	initializeDirectory(projectPath);
}

Resource * ObjectLibrary::getResource(boost::uuids::uuid uuid)
{
	// Loop through the list of resources until we find one with the correct ID
	// TODO: Use a better container for resource objects
	for (Resource* r : m_resources)
	{
		if (r->getId() == uuid)
		{
			return r;
		}
	}

	// If nothing was found, return a null pointer
	return nullptr;
}

void ObjectLibrary::initializeDirectory(std::string directory)
{
	printf("Loading project directory...\n");
	for (auto & p : fs::directory_iterator(directory))
	{
		std::string filePath = p.path().string();
		if (!endsWith(filePath, m_metaExtension))
		{
			initializeFile(filePath);
		}
	}
}

void ObjectLibrary::createMetaFile(int id, std::string metaFilePath)
{
	YAML::Node root;

	// Set the document's properties
	root["id"] = id;

	// Create the file emitter
	YAML::Emitter out;
	out.SetIndent(2);
	out.SetMapFormat(YAML::Block);
	out << root;
	std::ofstream file;
	file.open(metaFilePath);
	file << out.c_str();
	file.close();
}

bool ObjectLibrary::createMetaFile(std::string sourceFilePath, std::string metaFilePath)
{
	auto serializer = ResourceSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);
	if (serializer == nullptr)
	{
		printf("The file '%s' does not have a serializer associated with it.\n", sourceFilePath.c_str());
		return false;
	}

	YAML::Node root;
	YAML::Node resources = serializer->generateResourceNodes(sourceFilePath);

	root["resources"] = resources;

	// Create the file emitter
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
	m_resources.push_back(resource);
	/*if (resource->getId() > m_largestID)
	{
		m_largestID = resource->getId();
	}*/
}

Resource* ObjectLibrary::loadMetaFile(std::string sourceFilePath, std::string metaFilePath)
{
	return nullptr;
	/*int id;

	YAML::Node file = YAML::LoadFile(metaFilePath);
	id = file["id"].as<int>();

	Resource* r = new Resource(id, sourceFilePath, metaFilePath);
	return r;*/

	//YAML::Node file = YAML::LoadFile(metaFilePath);
	//YAML::Node resources = file["resources"];
	//if (!resources)
	//{
	//	return nullptr;
	//}

	//if (!resources.IsSequence())
	//{
	//	return nullptr;
	//}

	//for (unsigned int i = 0; i < resources.size(); i++)
	//{
	//	YAML::Node resource = resources[i];
	//	//boost::uuids::uuid uuid = resource["id"].as<boost::uuids::uuid>();
	//	//Resource* resource = nullptr;//new Resource(uuid, sourceFilePath, metaFilePath, UnknownResourceType);
	//}
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
	YAML::Node resourcesNode = file["resources"];
	if (!resourcesNode)
	{
		printf("The meta file '%s' does not have a resource node assigned it it. This file could not be parsed!\n", metaFilePath.c_str());
	}

	// Go through all the resource nodes in the file
	for (int i = 0; i < resourcesNode.size(); i++)
	{
		YAML::Node resourceNode = resourcesNode[i];

		// If no ID node is defined, exit out because it is required
		if (!resourceNode["id"])
		{
			printf("A node was skipped because it was missing an ID parameter.\n");
			continue;
		}

		// Get the ID
		boost::uuids::uuid resourceUuid = resourceNode["id"].as<boost::uuids::uuid>();

		// Find the serializer related to this source file object
		auto serializer = ResourceSerializerLibrary::getInstance().getTypeSerializer(sourceFilePath);

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
