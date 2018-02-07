#include "FileType.h"
#include "StringUtils.h"

FileType pathToFileType(std::string str)
{
	std::string extension = getFileExtension(str);
	if (extension == "derymaterial")
	{
		return MaterialFileType;
	}
	else if (extension == "fbx" || extension == "obj")
	{
		return MeshFileType;
	}
	//else if (extension == "bmp" || extension == "tga" || extension == "png")
	//{
	//	return TextureResourceType;
	//}
	//else if (extension == "txt")
	//{
	//	return TextResourceType;
	//}
	else
	{
		return UnknownFileType;
	}
}
