#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ImageFileSerializer : public FileTypeSerializer
	{
	public:
		ImageFileSerializer() {}
		~ImageFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}