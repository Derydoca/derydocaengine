#pragma once
#include "FileTypeSerializer.h"

class ShaderFileSerializer : public FileTypeSerializer
{
public:
	ShaderFileSerializer() {};
	~ShaderFileSerializer() {};

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
	void postLoadInitialize(Resource* resource);
};