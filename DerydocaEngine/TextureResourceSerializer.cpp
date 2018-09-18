#include "TextureResourceSerializer.h"
#include "Texture.h"
#include "Resource.h"

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
		std::string sourceFilePath = resource->getSourceFilePath();

		DerydocaEngine::Rendering::Texture* texture = new DerydocaEngine::Rendering::Texture(sourceFilePath);

		return texture;
	}

	ResourceType TextureResourceSerializer::getResourceType()
	{
		return ResourceType::TextureResourceType;
	}

}
