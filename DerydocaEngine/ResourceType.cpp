#include "ResourceType.h"
#include "StringUtils.h"

ResourceType pathToResourceType(std::string str)
{
	std::string extension = getFileExtension(str);
	if (extension == "derymaterial")
	{
		return MaterialResource;
	}
	else if (extension == "bmp" || extension == "tga" || extension == "png")
	{
		return TextureResource;
	}
	else if (extension == "txt")
	{
		return TextResource;
	}
	else
	{
		return UnknownResource;
	}
}