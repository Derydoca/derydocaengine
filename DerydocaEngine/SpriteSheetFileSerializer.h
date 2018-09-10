#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::FileSerializers {

	class SpriteSheetFileSerializer : public FileTypeSerializer
	{
	public:
		SpriteSheetFileSerializer() {}
		~SpriteSheetFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		FileType getFileType();
		Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}