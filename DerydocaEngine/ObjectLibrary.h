#pragma once
#include <string>
#include <vector>
#include "Resource.h"

class ResourceTypeSerializer;

class ObjectLibrary
{
public:
	static ObjectLibrary& getInstance()
	{
		static ObjectLibrary instance;
		return instance;
	}
	void operator=(ObjectLibrary const&) = delete;

	void initialize(std::string projectPath);
	std::string getMetaExtension() const { return m_metaExtension; }
	Resource* getResource(boost::uuids::uuid uuid);
private:
	void initializeDirectory(std::string directory);
	void initializeFile(std::string sourceFilePath);
	void createMetaFile(int id, std::string metaFilePath);
	bool createMetaFile(std::string sourceFilePath, std::string metaFilePath);
	void registerResource(Resource* resource);

	const std::string m_metaExtension = ".derymeta";
	std::vector<Resource*> m_resources;
	Resource* loadMetaFile(std::string sourceFilePath, std::string metaFilePath);
	unsigned int m_largestID = 0;

	ObjectLibrary() {}
	~ObjectLibrary() {}

	ObjectLibrary(ObjectLibrary const&) {}
};

