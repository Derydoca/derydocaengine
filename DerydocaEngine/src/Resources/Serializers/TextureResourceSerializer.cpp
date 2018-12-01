#include "Resources\Serializers\TextureResourceSerializer.h"
#include "Rendering\Texture.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources::Serializers
{

	TextureResourceSerializer::TextureResourceSerializer()
	{
	}

	TextureResourceSerializer::~TextureResourceSerializer()
	{
	}

	void * TextureResourceSerializer::deserialize(Resource * const& resource)
	{
		return nullptr;
	}

	std::shared_ptr<void> TextureResourceSerializer::deserializePointer(Resource * const& resource)
	{
		std::string sourceFilePath = resource->getSourceFilePath();

		auto texture = std::make_shared<Rendering::Texture>(sourceFilePath);

		return texture; 
	}

	ResourceType TextureResourceSerializer::getResourceType()
	{
		return ResourceType::TextureResourceType;
	}

}
