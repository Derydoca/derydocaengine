#pragma once
#include "FileTypeSerializer.h"

class FontFileSerializer : public FileTypeSerializer
{
public:
	FontFileSerializer();
	~FontFileSerializer();

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};
