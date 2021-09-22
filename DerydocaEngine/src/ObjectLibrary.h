#pragma once
#include <map>
#include <memory>
#include <string>
#include "Resources\Resource.h"
#include "Resources\ResourceTreeNode.h"
#include "Resources\ResourceType.h"

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

		void loadEngineResources(const std::filesystem::path& path);
		void loadProjectResources(const std::filesystem::path& path);
		void addResource(std::shared_ptr<Resources::Resource> resource)
		{
			m_resources[resource->getId()] = resource;
		}
		const uuids::uuid assetPathToId(const std::string& assetPath);
		const std::string& getMetaExtension() const { return m_metaExtension; }
		const std::filesystem::path& getProjectDirectory() const { return m_projectDirectory; }
		std::vector<std::shared_ptr<Resources::Resource>> getResourcesOfType(Resources::ResourceType resourceType);
		std::shared_ptr<Resources::ResourceTreeNode> getRootResourceTreeNode() const { return m_projectResourceRoot; }
		std::vector<sptr<Resources::Resource>> getResources(const std::string& metaFilePath);
		std::shared_ptr<Resources::Resource> getResource(std::string const& uuidString);
		std::shared_ptr<Resources::Resource> getResource(uuids::uuid const& uuid);
		void updateMetaFilesDirectory(const std::filesystem::path& directoryPath);
		void updateMetaFiles(std::string const& file);
		void loadDirectory(const std::filesystem::path& path);
		void loadFile(const std::filesystem::path& sourceFilePath);

		template<class GameComponentClass>
		std::shared_ptr<GameComponentClass> getComponent(const uuids::uuid& id)
		{
			auto component = getComponent(id);
			return std::static_pointer_cast<GameComponentClass>(component);
		}

		template<class resourceType>
		std::shared_ptr<resourceType> getResource(std::string const& uuidString)
		{
			uuids::uuid uuid(uuidString);
			std::shared_ptr<Resources::Resource> resource = getResource(uuid);
			return std::static_pointer_cast<resourceType>(resource);
		}

		template<class resourceType>
		std::shared_ptr<resourceType> getResource(const uuids::uuid& uuid)
		{
			std::shared_ptr<Resources::Resource> resource = getResource(uuid);
			return std::static_pointer_cast<resourceType>(resource);
		}

		template<class resourceObjectType>
		std::shared_ptr<resourceObjectType> getResourceObjectPointer(const std::string& id)
		{
			uuids::string_generator gen;
			uuids::uuid uuid = gen(id);
			return getResourceObjectPointer<resourceObjectType>(uuid);
		}

		template<class resourceObjectType>
		std::shared_ptr<resourceObjectType> getResourceObjectPointer(uuids::uuid const& id)
		{
			std::shared_ptr<Resources::Resource> resource = getResource(id);
			if (resource == nullptr)
			{
				return nullptr;
			}

			std::shared_ptr<resourceObjectType> pointer = std::static_pointer_cast<resourceObjectType>(resource->getResourceObjectPointer());
			return pointer;
		}
	private:
		ObjectLibrary();
		~ObjectLibrary() {}
		ObjectLibrary(ObjectLibrary const&) {}

		bool createMetaFile(std::string const& sourceFilePath, std::string const& metaFilePath);
		void registerResource(std::shared_ptr<Resources::Resource> resource);
		std::shared_ptr<Resources::ResourceTreeNode> getResourceTreeNode(const std::string& resourcePath);
		void loadResourceTree();

		const std::string m_metaExtension = ".dmeta";
		std::filesystem::path m_projectDirectory;
		std::map<uuids::uuid, std::shared_ptr<Resources::Resource>> m_resources;
		std::map<std::string, uuids::uuid> m_pathToIdMap;
		std::shared_ptr<Resources::ResourceTreeNode> m_projectResourceRoot;
	};

}
