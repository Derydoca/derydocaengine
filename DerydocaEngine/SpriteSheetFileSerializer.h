#pragma once
#include "FileTypeSerializer.h"

using namespace YAML;
using namespace std;

class SpriteSheetFileSerializer : public FileTypeSerializer
{
public:
	SpriteSheetFileSerializer();
	~SpriteSheetFileSerializer();

	Node generateResourceNodes(string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(Node const& resourceNode);
};

