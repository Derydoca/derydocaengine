#include "SpriteSheetResourceSerializer.h"
#include "SpriteSheet.h"

SpriteSheetResourceSerializer::SpriteSheetResourceSerializer()
{
}

SpriteSheetResourceSerializer::~SpriteSheetResourceSerializer()
{
}

void * SpriteSheetResourceSerializer::deserialize(Resource * const& resource)
{
	DerydocaEngine::UI::SpriteSheet* ss = new DerydocaEngine::UI::SpriteSheet();
	ss->LoadFromDisk(resource->getSourceFilePath());
	return ss;
}

ResourceType SpriteSheetResourceSerializer::getResourceType()
{
	return ResourceType::SpriteSheetType;
}
