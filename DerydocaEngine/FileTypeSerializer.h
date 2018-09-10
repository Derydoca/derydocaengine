#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "FileType.h"
#include "yaml-cpp\yaml.h"
#include "YamlTools.h"

struct Resource;

namespace DerydocaEngine::FileSerializers {

	class FileTypeSerializer
	{
	public:
		virtual ~FileTypeSerializer() {}
		virtual YAML::Node generateResourceNodes(std::string const& filePath) = 0;
		virtual FileType getFileType() = 0;
		virtual Resource* loadResourceFromMeta(YAML::Node const& resourceNode) = 0;
		virtual void postLoadInitialize(Resource* const& resource) {}

		friend boost::uuids::uuid generateUuid();
	};

}
