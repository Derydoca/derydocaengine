#pragma once
#include "FileTypeSerializer.h"

class MaterialSerializer : public FileTypeSerializer
{
public:
	MaterialSerializer();
	~MaterialSerializer();

	YAML::Node generateResourceNodes(std::string const& filePath);
	virtual FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
};

