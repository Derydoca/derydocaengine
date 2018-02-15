#pragma once
#include <string>

enum FileType
{
	UnknownFileType,
	MaterialFileType,
	MeshFileType,
	ImageFileType,
};

FileType pathToFileType(std::string str);