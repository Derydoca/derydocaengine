#pragma once
#include "FileTypeSerializer.h"

class ImageFileSerializer : public FileTypeSerializer
{
public:
	ImageFileSerializer();
	~ImageFileSerializer();

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};
