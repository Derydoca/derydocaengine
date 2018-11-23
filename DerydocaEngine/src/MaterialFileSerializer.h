#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class MaterialSerializer : public FileTypeSerializer
	{
	public:
		MaterialSerializer() {}
		~MaterialSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		virtual Files::FileType getFileType();
		Resources::Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

}