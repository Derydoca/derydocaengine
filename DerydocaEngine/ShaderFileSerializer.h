#pragma once
#include "FileTypeSerializer.h"

class ShaderFileSerializer : public FileTypeSerializer
{
public:
	ShaderFileSerializer() {};
	~ShaderFileSerializer() {};

	YAML::Node generateResourceNodes(std::string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	void postLoadInitialize(Resource* const& resource);
};