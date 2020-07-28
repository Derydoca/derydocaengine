#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Files\FileType.h"

namespace DerydocaEngine::Resources {
	struct Resource;
}

namespace DerydocaEngine::Files::Serializers {

	class FileTypeSerializer
	{
	public:
		virtual ~FileTypeSerializer() {}
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath) = 0;
		virtual Files::FileType getFileType() = 0;
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource) {}

		friend boost::uuids::uuid generateUuid();
	};

}
