#include "ResourceType.h"
#include "StringUtils.h"

ResourceType pathToResourceType(std::string str)
{
	std::string extension = getFileExtension(str);
	if (extension == "derymaterial")
	{
		return MaterialResourceType;
	}
	else if (extension == "fbx" || extension == "obj")
	{
		return ModelResourceType;
	}
	else if (extension == "bmp" || extension == "tga" || extension == "png")
	{
		return TextureResourceType;
	}
	else if (extension == "txt")
	{
		return TextResourceType;
	}
	else
	{
		return UnknownResourceType;
	}
}