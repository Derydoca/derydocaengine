#pragma once
#include "FileTypeSerializer.h"

class BezierPatchMeshFileSerializer : public FileTypeSerializer
{
public:
	BezierPatchMeshFileSerializer();
	~BezierPatchMeshFileSerializer();

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);
};

// Based on http://www.holmes3d.net/graphics/roffview/tools/patchoff/