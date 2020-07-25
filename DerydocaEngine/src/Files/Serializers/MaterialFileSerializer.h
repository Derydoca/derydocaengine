#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class MaterialSerializer : public FileTypeSerializer
	{
	public:
		MaterialSerializer() {}
		~MaterialSerializer() {}

		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		virtual Files::FileType getFileType();
	};

}
