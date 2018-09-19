#include "SpriteSheetFileSerializer.h"
#include "Resource.h"

namespace DerydocaEngine::FileSerializers {

	YAML::Node SpriteSheetFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node spriteSheetResource;
		spriteSheetResource["ID"] = generateUuid();

		resources.push_back(spriteSheetResource);

		return resources;
	}

	FileType SpriteSheetFileSerializer::getFileType()
	{
		return FileType::SpriteSheetFileType;
	}

	Resources::Resource * SpriteSheetFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::ResourceType::SpriteSheetType);
		return r;
	}

}
