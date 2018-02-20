#pragma once
#include <string>

enum FileType
{
	UnknownFileType,
	IgnoredFileType,
	MaterialFileType,
	MeshFileType,
	ImageFileType,
	LevelFileType,
	ShaderFileType,
};

FileType pathToFileType(std::string str);