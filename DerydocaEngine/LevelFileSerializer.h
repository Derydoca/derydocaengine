#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::FileSerializers {

	class LevelFileSerializer : public FileTypeSerializer
	{
	public:
		LevelFileSerializer() {};
		~LevelFileSerializer() {};

		YAML::Node generateResourceNodes(std::string const& filePath);
		FileType getFileType();
		Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}
