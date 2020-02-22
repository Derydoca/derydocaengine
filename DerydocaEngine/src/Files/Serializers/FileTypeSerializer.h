#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Files\FileType.h"
#include "yaml-cpp\yaml.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Resources {
	struct Resource;
}

namespace DerydocaEngine::Files::Serializers {

	class FileTypeSerializer
	{
	public:
		virtual ~FileTypeSerializer() {}
		virtual YAML::Node generateResourceNodes(std::string const& filePath) = 0;
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath) = 0;
		virtual Files::FileType getFileType() = 0;
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource) {}

		friend boost::uuids::uuid generateUuid();
	};

}
