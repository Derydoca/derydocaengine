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
	Resource* getResource(std::string uuidString);
	Resource* getResource(boost::uuids::uuid uuid);
	GameComponent* getComponent(boost::uuids::uuid id);
	Resource* getMetaFile(std::string sourceFilePath);
	void updateMetaFilesDirectory(std::string directory);
	void updateMetaFiles(std::string file);
	void loadDirectory(std::string directory);
	void loadFile(std::string sourceFilePath);

	void registerComponent(boost::uuids::uuid id, GameComponent* component);

	template<class resourceType>
	resourceType getResource(std::string uuidString)
	{
		boost::uuids::string_generator gen;
		boost::uuids::uuid uuid = gen(uuidString);
		Resource* resource = getResource(uuid);
		return static_cast<resourceType*>(resource);
	}
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

