#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ImageFileSerializer : public FileTypeSerializer
	{
	public:
		ImageFileSerializer() {}
		~ImageFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		Files::FileType getFileType();
		Resources::Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}