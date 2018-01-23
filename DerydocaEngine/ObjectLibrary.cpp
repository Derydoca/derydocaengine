#include "ObjectLibrary.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "yaml-cpp\yaml.h"
#include <iostream>
#include <fstream>
#include "StringUtils.h"

namespace fs = std::experimental::filesystem::v1;
std::string m_metaExtension = ".derymeta";

void ObjectLibrary::initialize(std::string projectPath)
{
	initializeDirectory(projectPath);
}

Resource * ObjectLibrary::getResource(int id)
{
	// Loop through the list of resources until we find one with the correct ID
	// TODO: Use a better container for resource objects
	for (Resource r : m_resources)
	{
		if (r.getId() == id)
		{
			return &r;
		}
	}

	// If nothing was found, return a null pointer
	return nullptr;
}

void ObjectLibrary::initializeDirectory(std::string directory)
{
	printf("Printing Directories");
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

void ObjectLibrary::registerResource(Resource resource)
{
	m_resources.push_back(resource);
	if (resource.getId() > m_largestID)
	{
		m_largestID = resource.getId();
	}
}

Resource* ObjectLibrary::loadMetaFile(std::string sourceFilePath, std::string metaFilePath)
{
	int id;

	YAML::Node file = YAML::LoadFile(metaFilePath);
	id = file["id"].as<int>();

	Resource* r = new Resource(id, sourceFilePath, metaFilePath);
	return r;
}

void ObjectLibrary::initializeFile(std::string sourceFilePath)
{
	std::string metaFilePath = sourceFilePath + m_metaExtension;

	// If the meta file does not exist
	if (!fs::exists(metaFilePath))
	{
		// Create the meta file
		createMetaFile(++m_largestID, metaFilePath);
	}

	// Load the meta file
	Resource* r = loadMetaFile(sourceFilePath, metaFilePath);

	// If a resource was successfully loaded, add it to the list of resources
	if (r)
	{
		registerResource(*r);
	}
}
