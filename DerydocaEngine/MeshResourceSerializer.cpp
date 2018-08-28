#include "MeshResourceSerializer.h"
#include "MeshResource.h"
#include "Mesh.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

MeshResourceSerializer::MeshResourceSerializer()
{
}

MeshResourceSerializer::~MeshResourceSerializer()
{
}

void * MeshResourceSerializer::deserialize(Resource * resource)
{
	MeshResource* mr = (MeshResource*)resource;
	auto file = aiImportFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	Mesh* m = new Mesh();
	m->setFlags(mr->getFlags());
	m->loadFromFile(resource->getSourceFilePath().c_str(), mr->getMeshIndex());

	return m;
}

ResourceType MeshResourceSerializer::getResourceType()
{
	return ResourceType::MeshResourceType;
}
