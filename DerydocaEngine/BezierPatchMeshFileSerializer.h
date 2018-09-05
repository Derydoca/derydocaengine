#pragma once
#include "FileTypeSerializer.h"

class BezierPatchMeshFileSerializer : public FileTypeSerializer
{
public:
	BezierPatchMeshFileSerializer();
	~BezierPatchMeshFileSerializer();

	YAML::Node generateResourceNodes(std::string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
};

// Based on http://www.holmes3d.net/graphics/roffview/tools/patchoff/