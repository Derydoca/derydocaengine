#pragma once
#include <string>

enum ResourceType
{
	UnknownResource,
	MaterialResource,
	TextureResource,
	TextResource,
};

ResourceType pathToResourceType(std::string str);