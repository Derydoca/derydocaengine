#include "EnginePch.h"
#include "Resources\Serializers\SpriteSheetResourceSerializer.h"
#include "UI\Spritesheet.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<Object> SpriteSheetResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		std::shared_ptr<UI::SpriteSheet> ss = std::make_shared<UI::SpriteSheet>();
		ss->LoadFromDisk(resource->getSourceFilePath());
		return ss;
	}

}
