#include "EnginePch.h"
#include "Files\Serializers\LevelFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node LevelFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	Files::FileType LevelFileSerializer::getFileType()
	{
		return Files::FileType::LevelFileType;
	}

	std::shared_ptr<Resources::Resource> LevelFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::Resource>();
		r->setType(Resources::LevelResourceType);
		return r;
	}

}
