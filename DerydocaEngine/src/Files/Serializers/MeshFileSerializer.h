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

		YAML::Node generateResourceNodes(std::string const& filePath);
		virtual std::vector<std::shared_ptr<Resources::Resource>> generateResources(const std::string& assetPath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);

	private:
		bool findMeshResourceNode(YAML::Node const& root, unsigned int const& index, aiString const& name, YAML::Node & resourceNode);
	};

}