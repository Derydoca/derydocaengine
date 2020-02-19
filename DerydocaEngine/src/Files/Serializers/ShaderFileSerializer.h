#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ShaderFileSerializer : public FileTypeSerializer
	{
	public:
		ShaderFileSerializer() {};
		~ShaderFileSerializer() {};

		YAML::Node generateResourceNodes(std::string const& filePath);
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource);
	};

}
