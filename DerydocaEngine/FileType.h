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
	BezierPatchMeshType,
	FontType,
	RasterFontType,
	SpriteSheetFileType,
};

FileType pathToFileType(std::string str);