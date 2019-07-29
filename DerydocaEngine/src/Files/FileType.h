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
		GlslShaderFileType,
		BezierPatchMeshType,
		FontType,
		RasterFontType,
		SpriteSheetFileType,
	};

	FileType pathToFileType(std::string const& str);

}
