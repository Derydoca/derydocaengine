#pragma once
#include <string>

namespace DerydocaEngine::Files
{

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

	FileType pathToFileType(std::string const& str);

}
