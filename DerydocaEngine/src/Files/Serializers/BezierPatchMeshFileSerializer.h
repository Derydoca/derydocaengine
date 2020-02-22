#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class BezierPatchMeshFileSerializer : public FileTypeSerializer
	{
	public:
		BezierPatchMeshFileSerializer() {}
		~BezierPatchMeshFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
	};

	// Based on http://www.holmes3d.net/graphics/roffview/tools/patchoff/
}
