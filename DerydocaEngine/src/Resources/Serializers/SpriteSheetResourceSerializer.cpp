#include "Resources\Serializers\SpriteSheetResourceSerializer.h"
#include "UI\Spritesheet.h"

namespace DerydocaEngine::Resources::Serializers
{

	SpriteSheetResourceSerializer::SpriteSheetResourceSerializer()
	{
	}

	SpriteSheetResourceSerializer::~SpriteSheetResourceSerializer()
	{
	}

	std::shared_ptr<void> SpriteSheetResourceSerializer::deserializePointer(Resource * const & resource)
	{
		std::shared_ptr<UI::SpriteSheet> ss = std::make_shared<UI::SpriteSheet>();
		ss->LoadFromDisk(resource->getSourceFilePath());
		return ss;
	}

	ResourceType SpriteSheetResourceSerializer::getResourceType()
	{
		return ResourceType::SpriteSheetType;
	}

}
