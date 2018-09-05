#pragma once
#include "FileTypeSerializer.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class MeshSerializer : public FileTypeSerializer
{
public:
	MeshSerializer();
	~MeshSerializer();

	YAML::Node generateResourceNodes(std::string const& filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node const& resourceNode);

private:
	bool findMeshResourceNode(YAML::Node const& root, unsigned int const& index, aiString const& name, YAML::Node & resourceNode);
};

