#include "TextureResourceSerializer.h"
#include "Texture.h"
#include "Resource.h"

TextureResourceSerializer::TextureResourceSerializer()
{
}


TextureResourceSerializer::~TextureResourceSerializer()
{
}

void * TextureResourceSerializer::deserialize(Resource * const& resource)
{
	std::string sourceFilePath = resource->getSourceFilePath();

	Texture* texture = new Texture(sourceFilePath);

	return texture;
}

ResourceType TextureResourceSerializer::getResourceType()
{
	return ResourceType::TextureResourceType;
}
