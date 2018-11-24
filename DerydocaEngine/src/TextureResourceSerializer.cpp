#include "TextureResourceSerializer.h"
#include "Rendering\Texture.h"
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

		Rendering::Texture* texture = new Rendering::Texture(sourceFilePath);

		return texture;
	}

	ResourceType TextureResourceSerializer::getResourceType()
	{
		return ResourceType::TextureResourceType;
	}

}
