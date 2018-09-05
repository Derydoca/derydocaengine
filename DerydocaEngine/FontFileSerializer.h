#pragma once
#include "FileTypeSerializer.h"

class FontFileSerializer : public FileTypeSerializer
{
public:
	FontFileSerializer();
	~FontFileSerializer();

	YAML::Node generateResourceNodes(std::string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
};
