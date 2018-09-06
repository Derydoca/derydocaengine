#pragma once
#include <string>
#include <map>
#include "Resource.h"
#include <boost/uuid/uuid.hpp>

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

	void initialize(std::string const& engineResourcesPath, std::string const& projectPath);
	std::string getMetaExtension() const { return m_metaExtension; }
	Resource* getResource(std::string const& uuidString);
	Resource* getResource(boost::uuids::uuid const& uuid);
	GameComponent* getComponent(boost::uuids::uuid const& id);
	Resource* getMetaFile(std::string const& sourceFilePath);
	void updateMetaFilesDirectory(std::string const& directory);
	void updateMetaFiles(std::string const& file);
	void loadDirectory(std::string const& directory);
	void loadFile(std::string const& sourceFilePath);

	void registerComponent(boost::uuids::uuid const& id, GameComponent* const& component);

	template<class resourceType>
	resourceType getResource(std::string const& uuidString)
	{
		boost::uuids::string_generator gen;
		boost::uuids::uuid uuid = gen(uuidString);
		Resource* resource = getResource(uuid);
		return static_cast<resourceType*>(resource);
	}

	template<class resourceObjectType>
	resourceObjectType* getResourceObject(boost::uuids::uuid const& id)
	{
		Resource* resource = getResource(id);
		if (resource == nullptr)
		{
			return nullptr;
		}

		return static_cast<resourceObjectType*>(resource->getResourceObject());
	}
private:
	ObjectLibrary() {}
	~ObjectLibrary() {}
	ObjectLibrary(ObjectLibrary const&) {}

	bool createMetaFile(std::string const& sourceFilePath, std::string const& metaFilePath);
	void registerResource(Resource* const& resource);

	const std::string m_metaExtension = ".derymeta";
	std::map<boost::uuids::uuid, Resource*> m_resources;
	std::map<boost::uuids::uuid, GameComponent*> m_sceneComponents;
};

