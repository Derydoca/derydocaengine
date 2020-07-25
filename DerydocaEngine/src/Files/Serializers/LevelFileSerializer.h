#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class LevelFileSerializer : public FileTypeSerializer
	{
	public:
		LevelFileSerializer() {};
		~LevelFileSerializer() {};

		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
	};

}
