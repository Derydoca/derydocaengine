#pragma once
#include "FileTypeSerializer.h"

class RasterFontFileSerializer : public FileTypeSerializer
{
public:
	RasterFontFileSerializer();
	~RasterFontFileSerializer();

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};

