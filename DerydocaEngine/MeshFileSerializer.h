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

	YAML::Node generateResourceNodes(std::string filePath);
	FileType getFileType();
	Resource* loadResourceFromMeta(YAML::Node resourceNode);

private:
	bool findMeshResourceNode(YAML::Node root, unsigned int index, aiString name, YAML::Node& resourceNode);
};

