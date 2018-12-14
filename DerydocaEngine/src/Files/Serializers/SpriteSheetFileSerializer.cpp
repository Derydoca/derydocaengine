#include "EnginePch.h"
#include "Files\Serializers\SpriteSheetFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node SpriteSheetFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node spriteSheetResource;
		spriteSheetResource["ID"] = generateUuid();

		resources.push_back(spriteSheetResource);

		return resources;
	}

	Files::FileType SpriteSheetFileSerializer::getFileType()
	{
		return Files::FileType::SpriteSheetFileType;
	}

	std::shared_ptr<Resources::Resource> SpriteSheetFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::Resource>();
		r->setType(Resources::ResourceType::SpriteSheetType);
		return r;
	}

}
