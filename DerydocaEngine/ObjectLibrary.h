#pragma once
#include <string>
#include <map>
#include "Resource.h"
#include "boost/uuid/uuid.hpp"

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
	ObjectLibrary() {}
	~ObjectLibrary() {}
	ObjectLibrary(ObjectLibrary const&) {}

	bool createMetaFile(std::string sourceFilePath, std::string metaFilePath);
	void initializeDirectory(std::string directory);
	void initializeFile(std::string sourceFilePath);
	void registerResource(Resource* resource);

	const std::string m_metaExtension = ".derymeta";
	std::map<boost::uuids::uuid, Resource*> m_resources;
};

