#pragma once
#include "MaterialResourceSerializer.h"

class TextureResourceSerializer : public MaterialResourceSerializer
{
public:
	TextureResourceSerializer();
	~TextureResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};
