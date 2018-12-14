#include "EnginePch.h"
#include "Resources\Serializers\TextureResourceSerializer.h"
#include "Rendering\Texture.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> TextureResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		std::string sourceFilePath = resource->getSourceFilePath();

		auto texture = std::make_shared<Rendering::Texture>(sourceFilePath);

		return texture; 
	}

}
