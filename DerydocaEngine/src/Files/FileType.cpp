#include "Files\FileType.h"
#include "StringUtils.h"

namespace DerydocaEngine::Files
{

	FileType pathToFileType(std::string const& str)
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
		else if (extension == "bmp" || extension == "tga" || extension == "png" || extension == "jpg" || extension == "jpeg")
		{
			return ImageFileType;
		}
		else if (extension == "derylevel")
		{
			return LevelFileType;
		}
		else if (extension == "vs")
		{
			return ShaderFileType;
		}
		else if (extension == "bpm")
		{
			return BezierPatchMeshType;
		}
		else if (extension == "ttf")
		{
			return FontType;
		}
		else if (extension == "dfont")
		{
			return RasterFontType;
		}
		else if (extension == "txt" || extension == "fs" || extension == "gs" || extension == "tes" || extension == "tcs")
		{
			return IgnoredFileType;
		}
		else if (extension == "dsprites")
		{
			return SpriteSheetFileType;
		}
		else
		{
			return UnknownFileType;
		}
	}

}
