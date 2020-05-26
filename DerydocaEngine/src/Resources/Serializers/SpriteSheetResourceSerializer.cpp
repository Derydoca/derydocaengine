#include "EnginePch.h"
#include "Resources\Serializers\SpriteSheetResourceSerializer.h"
#include "UI\Spritesheet.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> SpriteSheetResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		std::shared_ptr<UI::SpriteSheet> ss = std::make_shared<UI::SpriteSheet>();
		ss->LoadFromSerializedFile(resource->getSourceFilePath());
		return ss;
	}

}
