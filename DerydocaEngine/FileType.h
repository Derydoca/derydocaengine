#pragma once
#include <string>

enum FileType
{
	UnknownFileType,
	MaterialFileType,
	MeshFileType,
};

FileType pathToFileType(std::string str);