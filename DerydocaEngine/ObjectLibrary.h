#pragma once
#include <string>
#include <map>
#include "Resource.h"
#include "boost/uuid/uuid.hpp"

class GameComponent;
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

	void initialize(std::string engineResourcesPath, std::string projectPath);
	std::string getMetaExtension() const { return m_metaExtension; }
	Resource* getResource(boost::uuids::uuid uuid);
	GameComponent* getComponent(boost::uuids::uuid id);
	void initializeDirectory(std::string directory);
	void initializeFile(std::string sourceFilePath);

	void registerComponent(boost::uuids::uuid id, GameComponent* component);
private:
	ObjectLibrary() {}
	~ObjectLibrary() {}
	ObjectLibrary(ObjectLibrary const&) {}

	bool createMetaFile(std::string sourceFilePath, std::string metaFilePath);
	void registerResource(Resource* resource);

	const std::string m_metaExtension = ".derymeta";
	std::map<boost::uuids::uuid, Resource*> m_resources;
	std::map<boost::uuids::uuid, GameComponent*> m_sceneComponents;
};

