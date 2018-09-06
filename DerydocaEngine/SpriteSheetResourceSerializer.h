#pragma once
#include "ResourceSerializer.h"

class SpriteSheetResourceSerializer : public ResourceSerializer
{
public:
	SpriteSheetResourceSerializer();
	~SpriteSheetResourceSerializer();

	void* deserialize(Resource* const& resource);
	ResourceType getResourceType();
};

