#pragma once
#include "FileTypeSerializer.h"

class LevelFileSerializer : public FileTypeSerializer
{
public:
	LevelFileSerializer() {};
	~LevelFileSerializer() {};

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};