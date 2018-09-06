#pragma once
#include "MaterialResourceSerializer.h"

class TextureResourceSerializer : public MaterialResourceSerializer
{
public:
	TextureResourceSerializer();
	~TextureResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};
