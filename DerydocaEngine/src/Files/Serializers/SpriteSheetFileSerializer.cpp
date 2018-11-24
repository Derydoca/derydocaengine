#include "Files\Serializers\SpriteSheetFileSerializer.h"
#include "Resource.h"

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

	Resources::Resource * SpriteSheetFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::ResourceType::SpriteSheetType);
		return r;
	}

}
