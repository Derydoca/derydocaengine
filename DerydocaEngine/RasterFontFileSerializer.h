#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::FileSerializers {

	class RasterFontFileSerializer : public FileTypeSerializer
	{
	public:
		RasterFontFileSerializer() {}
		~RasterFontFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		FileType getFileType();
		Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}
