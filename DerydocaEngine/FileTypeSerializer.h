#pragma once
#include <string>
#include "Resource.h"
#include "FileType.h"
#include "yaml-cpp\yaml.h"
#include <vector>
#include "YamlTools.h"

#include <boost/uuid/uuid.hpp>

class FileTypeSerializer
{
public:
	virtual YAML::Node generateResourceNodes(std::string const& filePath) = 0;
	virtual FileType getFileType() = 0;
	virtual Resource* loadResourceFromMeta(YAML::Node const& resourceNode) = 0;
	virtual void postLoadInitialize(Resource* const& resource) {}

	friend boost::uuids::uuid generateUuid();
};