#pragma once
#include "Files\Serializers\FileTypeSerializer.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

namespace DerydocaEngine::Files::Serializers {

	class MeshSerializer : public FileTypeSerializer
	{
	public:
		MeshSerializer() {}
		~MeshSerializer() {}

		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();

	};

}