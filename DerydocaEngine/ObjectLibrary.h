#pragma once
#include <string>
#include <vector>
#include "Resource.h"

class ObjectLibrary
{
public:
	ObjectLibrary() {}
	~ObjectLibrary() {}

	void initialize(std::string projectPath);
	std::string getMetaExtension() const { return m_metaExtension; }
	Resource* getResource(int id);
private:
	void initializeDirectory(std::string directory);
	void initializeFile(std::string sourceFilePath);
	void createMetaFile(int id, std::string metaFilePath);
	void registerResource(Resource resource);

	const std::string m_metaExtension = ".derymeta";
	std::vector<Resource> m_resources;
	Resource* loadMetaFile(std::string sourceFilePath, std::string metaFilePath);
	unsigned int m_largestID = 0;

	ObjectLibrary(ObjectLibrary const&) {}
};

