#pragma once
#include <string>

enum ResourceType
{
	UnknownResourceType,
	MaterialResourceType,
	TextureResourceType,
	TextResourceType,
	ModelResourceType,
	MeshResourceType,
};

ResourceType pathToResourceType(std::string str);