#include "SpriteSheetFileSerializer.h"



SpriteSheetFileSerializer::SpriteSheetFileSerializer()
{
}


SpriteSheetFileSerializer::~SpriteSheetFileSerializer()
{
}

Node SpriteSheetFileSerializer::generateResourceNodes(string const& filePath)
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

Resource * SpriteSheetFileSerializer::loadResourceFromMeta(Node const& resourceNode)
{
	Resource* r = new Resource();
	r->setType(ResourceType::SpriteSheetType);
	return r;
}
