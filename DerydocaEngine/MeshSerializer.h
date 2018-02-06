#pragma once
#include "ResourceTypeSerializer.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class MeshSerializer : public ResourceTypeSerializer
{
public:
	MeshSerializer();
	~MeshSerializer();

	ResourceType getResourceType();
	void* deserialize(Resource* resource);
	YAML::Node generateResourceNodes(std::string filePath);
	Resource* loadResourceFromMeta(YAML::Node resourceNode);

private:
	bool findMeshResourceNode(YAML::Node root, unsigned int index, aiString name, YAML::Node& resourceNode);
};

