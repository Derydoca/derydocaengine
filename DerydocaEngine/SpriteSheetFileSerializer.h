#pragma once
#include "FileTypeSerializer.h"

class SpriteSheetFileSerializer : public FileTypeSerializer
{
public:
	SpriteSheetFileSerializer();
	~SpriteSheetFileSerializer();

	YAML::Node generateResourceNodes(std::string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
};

