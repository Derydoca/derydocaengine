#include "EnginePch.h"
#include "ObjectLibrary.h"
#include <iostream>
#include <string>
#include "Files\Serializers\FileSerializerLibrary.h"
#include "Helpers\StringUtils.h"
#include "Files\FileType.h"
#include "Files\FileUtils.h"

namespace DerydocaEngine
{

	ObjectLibrary::ObjectLibrary() :
		m_projectResourceRoot(std::make_shared<Resources::ResourceTreeNode>("__projectRoot__"))
	{
	}

	void ObjectLibrary::loadEngineResources(const std::filesystem::path& path)
	{
		D_LOG_TRACE("Loading engine files: {}", path.string());
		m_engineResourceDirectoryRoot = std::make_shared<Directory>();
		m_engineResourceDirectoryRoot->Path = path;
		loadDirectory(m_engineResourceDirectoryRoot);
	}

	void ObjectLibrary::loadProjectResources(const std::filesystem::path& path)
	{
		D_LOG_TRACE("Updating meta files: {}", path.string());
		updateMetaFilesDirectory(path);
		D_LOG_TRACE("Loading project files: {}", path.string());
		m_projectResourceDirectoryRoot = std::make_shared<Directory>();
		m_projectResourceDirectoryRoot->Path = path;
		loadDirectory(m_projectResourceDirectoryRoot);
		m_projectDirectory = path;

		loadResourceTree();
	}

	std::shared_ptr<Resources::Resource> ObjectLibrary::getResource(std::string const& uuidString)
	{
		uuids::uuid uuid(uuidString);
		return getResource(uuid);
	}

	std::shared_ptr<Resources::Resource> ObjectLibrary::getResource(uuids::uuid const& uuid)
	{
		// Find a resource with a matching uuid
		auto search = m_resources.find(uuid);
		if (search != m_resources.end())
		{
			// If we found it, return the reference to the resource
			return search->second;
		}
		else
		{
			// If it was not in the collection, return null
			return nullptr;
		}
	}

	const uuids::uuid ObjectLibrary::assetPathToId(const std::string& assetPath)
	{
		auto find = m_pathToIdMap.find(assetPath);
		if (find != m_pathToIdMap.end())
		{
			return find->second;
		}
		return uuids::uuid();
	}

	std::vector<std::shared_ptr<Resources::Resource>> ObjectLibrary::getResourcesOfType(Resources::ResourceType resourceType)
	{
		// Consider creating an unordered map of resources with the key of resource type for a more inexpensive lookup time
		auto matchingResources = std::vector<std::shared_ptr<Resources::Resource>>();
		for (auto resource : m_resources)
		{
			if (resource.second->getType() == resourceType)
			{
				matchingResources.push_back(resource.second);
			}
		}

		// Sort the resources by name
		std::sort(
			matchingResources.begin(), 
			matchingResources.end(),
			[](const std::shared_ptr<Resources::Resource> r1, const std::shared_ptr<Resources::Resource> r2) -> bool {
				return r1->getName().compare(r2->getName()) < 0;
		});

		return matchingResources;
	}

	std::vector<sptr<Resources::Resource>> ObjectLibrary::getResources(const std::string& metaFilePath)
	{
		return Files::Utils::ReadFromDisk2<std::vector<sptr<Resources::Resource>>>(metaFilePath);
	}

	void ObjectLibrary::updateMetaFilesDirectory(const std::filesystem::path& directoryPath)
	{
		std::filesystem::directory_iterator it{ directoryPath };
		while (it != std::filesystem::directory_iterator{})
		{
			if (is_directory(it->path()))
			{
				auto pathStr = it->path().string();
				updateMetaFilesDirectory(pathStr);
			}
			else
			{
				if (!endsWith(it->path().string(), m_metaExtension))
				{
					auto pathStr = it->path().string();
					updateMetaFiles(pathStr);
				}
			}
			it++;
		}
	}

	void ObjectLibrary::updateMetaFiles(std::string const& sourceFilePath)
	{
		std::string metaFilePath = sourceFilePath + m_metaExtension;

		// If the meta file does not exist
		if (!std::filesystem::exists(metaFilePath))
		{
			// Create the meta file
			if (!createMetaFile(sourceFilePath, metaFilePath))
			{
				// If the meta file was not be created, skip this file
				return;
			}
		}

		auto resources = getResources(metaFilePath);
		for (auto resource : resources)
		{
			if (resource == NULL)
			{
				D_LOG_ERROR("RESOURCE IS NULL");
				continue;
			}
			resource->setFilePaths(sourceFilePath, metaFilePath);
			registerResource(resource);
		}

	}

	void ObjectLibrary::loadDirectory(std::shared_ptr<Directory> directory)
	{
		//std::filesystem::directory_iterator it{ directory.Path };
		//while (it != std::filesystem::directory_iterator{})
		for (auto const& dir_entry : std::filesystem::directory_iterator{ directory->Path })
		{
			if (dir_entry.is_directory())
			{
				auto childDir = std::make_shared<Directory>();
				childDir->Path = dir_entry.path();

				loadDirectory(childDir);

				directory->Children.push_back(childDir);
			}
			else
			{
				if (dir_entry.path().has_extension() && !endsWith(dir_entry.path().string(), m_metaExtension))
				{
					auto f = std::make_shared<File>();
					f->Path = dir_entry.path();
					directory->Files.push_back(f);

					loadFile(dir_entry.path());
				}
			}
		}
	}

	bool ObjectLibrary::createMetaFile(std::string const& assetPath, std::string const& metaFilePath)
	{
		Files::FileType fileType = Files::pathToFileType(assetPath);

		// If this is a file type we are expected to ignore, lets ignore it
		if (fileType == Files::FileType::IgnoredFileType)
		{
			return false;
		}

		// Find the serializer for this file type
		auto serializer = Files::Serializers::FileSerializerLibrary::getInstance().getTypeSerializer(fileType);

		// If the serializer was not found, abort and return false
		if (serializer == nullptr)
		{
			D_LOG_ERROR("The file '{}' does not have a serializer associated with it.", assetPath);
			return false;
		}

		auto resources = serializer->generateResources(assetPath);
		{
			std::ofstream fs(metaFilePath);
			cereal::JSONOutputArchive oarchive(fs);
			oarchive(SERIALIZE(resources));
		}

		return true;
	}

	void ObjectLibrary::registerResource(std::shared_ptr<Resources::Resource> resource)
	{
		resource->postLoadInitialize();

		// Load the resource into the map
		m_resources.insert(std::pair<uuids::uuid, std::shared_ptr<Resources::Resource>>(resource->getId(), resource));
		m_pathToIdMap.insert(std::pair<std::string, uuids::uuid>(resource->getSourceFilePath(), resource->getId()));
	}

	std::shared_ptr<Resources::ResourceTreeNode> ObjectLibrary::getResourceTreeNode(const std::string& resourcePath)
	{
		auto path = std::filesystem::path(resourcePath);

		bool passedFirstDir = false;
		std::istringstream ss(path.parent_path().string());
		std::string part;
		auto currentNode = m_projectResourceRoot;
		
		while (std::getline(ss, part, '\\'))
		{
			if (part.length() == 0 || part == "." || part == "..")
			{
				continue;
			}
		
			if (!passedFirstDir)
			{
				passedFirstDir = true;
				continue;
			}
		
			auto child = currentNode->getChild(part);
			if (child == nullptr)
			{
				child = std::make_shared<Resources::ResourceTreeNode>(part);
				currentNode->addChild(child);
			}
			currentNode = child;
		}

		return currentNode;
	}

	void ObjectLibrary::loadResourceTree()
	{
		m_projectResourceRoot->clear();

		for (auto resource : m_resources)
		{
			auto node = getResourceTreeNode(resource.second->getSourceFilePath());
			node->addResource(resource.second);
		}

		m_projectResourceRoot->sort();
	}

	void ObjectLibrary::loadFile(const std::filesystem::path& sourceFilePath)
	{
		std::filesystem::path metaFilePath(sourceFilePath.string() + m_metaExtension);

		// If the meta file does not exist, skip loading this resource
		if (!std::filesystem::exists(metaFilePath))
		{
			return;
		}

		// Load the meta file
		std::vector<std::shared_ptr<Resources::Resource>> resources;
		{
			try
			{
				std::string metaFilePathStr = metaFilePath.string();
				std::ifstream fs(metaFilePathStr);
				cereal::JSONInputArchive iarchive(fs);
				iarchive(resources);

				for (auto resource : resources)
				{
					resource->setFilePaths(sourceFilePath, metaFilePath);
					registerResource(resource);
				}
			}
			catch (const std::exception & ex)
			{
				std::string metaFilePathStr = metaFilePath.string();
				D_LOG_ERROR("An error occurred while attempting to load this meta file: {}\n{}", metaFilePathStr, ex.what());
			}
			catch (...)
			{
				std::string metaFilePathStr = metaFilePath.string();
				D_LOG_ERROR("An unknown error occurred while attempting to load this meta file: {}", metaFilePathStr);
			}
		}
	}

}
