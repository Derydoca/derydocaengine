#pragma once
#include <map>
#include <memory>
#include <string>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include "Resource.h"

namespace DerydocaEngine::Components {
	class GameComponent;
}

namespace DerydocaEngine
{

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
		Resources::Resource* getResource(std::string const& uuidString);
		Resources::Resource* getResource(boost::uuids::uuid const& uuid);
		Components::GameComponent* getComponent(boost::uuids::uuid const& id);
		Resources::Resource* getMetaFile(std::string const& sourceFilePath);
		void updateMetaFilesDirectory(std::string const& directory);
		void updateMetaFiles(std::string const& file);
		void loadDirectory(std::string const& directory);
		void loadFile(std::string const& sourceFilePath);

		void registerComponent(boost::uuids::uuid const& id, Components::GameComponent* const& component);

		template<class resourceType>
		resourceType getResource(std::string const& uuidString)
		{
			boost::uuids::string_generator gen;
			boost::uuids::uuid uuid = gen(uuidString);
			Resources::Resource* resource = getResource(uuid);
			return static_cast<resourceType*>(resource);
		}

		template<class resourceObjectType>
		resourceObjectType* getResourceObject(boost::uuids::uuid const& id)
		{
			Resources::Resource* resource = getResource(id);
			if (resource == nullptr)
			{
				return nullptr;
			}

			return static_cast<resourceObjectType*>(resource->getResourceObject());
		}

		template<class resourceObjectType>
		std::shared_ptr<resourceObjectType> getResourceObjectPointer(boost::uuids::uuid const& id)
		{
			Resources::Resource* resource = getResource(id);
			if (resource == nullptr)
			{
				return nullptr;
			}

			std::shared_ptr<resourceObjectType> pointer = std::static_pointer_cast<resourceObjectType>(resource->getResourceObjectPointer());
			return pointer;
		}
	private:
		ObjectLibrary() {}
		~ObjectLibrary() {}
		ObjectLibrary(ObjectLibrary const&) {}

		bool createMetaFile(std::string const& sourceFilePath, std::string const& metaFilePath);
		void registerResource(Resources::Resource* const& resource);

		const std::string m_metaExtension = ".derymeta";
		std::map<boost::uuids::uuid, Resources::Resource*> m_resources;
		std::map<boost::uuids::uuid, Components::GameComponent*> m_sceneComponents;
	};

}
