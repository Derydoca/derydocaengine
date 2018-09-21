#include "SpriteSheetResourceSerializer.h"
#include "SpriteSheet.h"

namespace DerydocaEngine::Resources::Serializers
{

	SpriteSheetResourceSerializer::SpriteSheetResourceSerializer()
	{
	}

	SpriteSheetResourceSerializer::~SpriteSheetResourceSerializer()
	{
	}

	void * SpriteSheetResourceSerializer::deserialize(Resource * const& resource)
	{
		UI::SpriteSheet* ss = new UI::SpriteSheet();
		ss->LoadFromDisk(resource->getSourceFilePath());
		return ss;
	}

	ResourceType SpriteSheetResourceSerializer::getResourceType()
	{
		return ResourceType::SpriteSheetType;
	}

}
