#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class CubemapSerializer : public FileTypeSerializer
	{
	public:
		CubemapSerializer() {}
		~CubemapSerializer() {}

		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		virtual Files::FileType getFileType();
	};

}
