#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "FileType.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"

namespace DerydocaEngine::Resources {
	struct Resource;
}

namespace DerydocaEngine::Files::Serializers {

	class FileTypeSerializer
	{
	public:
		virtual ~FileTypeSerializer() {}
		virtual YAML::Node generateResourceNodes(std::string const& filePath) = 0;
		virtual Files::FileType getFileType() = 0;
		virtual Resources::Resource* loadResourceFromMeta(YAML::Node const& resourceNode) = 0;
		virtual void postLoadInitialize(Resources::Resource* const& resource) {}

		friend boost::uuids::uuid generateUuid();
	};

}
