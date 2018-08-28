#pragma once
#include "ResourceSerializer.h"

class SpriteSheetResourceSerializer : public ResourceSerializer
{
public:
	SpriteSheetResourceSerializer();
	~SpriteSheetResourceSerializer();

	void* deserialize(Resource* resource);
	ResourceType getResourceType();
};

