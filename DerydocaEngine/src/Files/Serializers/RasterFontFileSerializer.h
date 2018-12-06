#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class RasterFontFileSerializer : public FileTypeSerializer
	{
	public:
		RasterFontFileSerializer() {}
		~RasterFontFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}
