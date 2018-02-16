#pragma once
#include <string>

enum FileType
{
	UnknownFileType,
	MaterialFileType,
	MeshFileType,
	ImageFileType,
	LevelFileType,
};

FileType pathToFileType(std::string str);