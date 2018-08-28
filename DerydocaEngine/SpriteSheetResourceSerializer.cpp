#include "SpriteSheetResourceSerializer.h"
#include "SpriteSheet.h"

SpriteSheetResourceSerializer::SpriteSheetResourceSerializer()
{
}

SpriteSheetResourceSerializer::~SpriteSheetResourceSerializer()
{
}

void * SpriteSheetResourceSerializer::deserialize(Resource * resource)
{
	SpriteSheet* ss = new SpriteSheet();
	ss->LoadFromDisk(resource->getSourceFilePath());
	return ss;
}

ResourceType SpriteSheetResourceSerializer::getResourceType()
{
	return ResourceType::SpriteSheetType;
}
