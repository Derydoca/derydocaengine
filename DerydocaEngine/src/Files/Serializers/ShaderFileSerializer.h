#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ShaderFileSerializer : public FileTypeSerializer
	{
	public:
		ShaderFileSerializer() {};
		~ShaderFileSerializer() {};

		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource);
	};

}
