#include "MeshFileSerializer.h"
#include "MeshResource.h"
#include "Mesh.h"

MeshSerializer::MeshSerializer()
{
}


MeshSerializer::~MeshSerializer()
{
}

FileType MeshSerializer::getFileType()
{
	return FileType::MeshFileType;
}

YAML::Node MeshSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	auto file = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	for (unsigned int i = 0; i < file->mNumMeshes; i++)
	{
		aiMesh* mesh = file->mMeshes[i];

		// Try to find a matching node that already exists in the file
		YAML::Node resourceNode;
		if (!findMeshResourceNode(resources, i, mesh->mName, resourceNode))
		{
			// If one was not found, initialize the ID and type
			resourceNode["id"] = generateUuid();
			resourceNode["type"] = "Mesh";
		}

		// Set object properties
		resourceNode["index"] = i;
		resourceNode["name"] = mesh->mName.C_Str();

		// Add it to the resource node
		resources.push_back(resourceNode);
	}

	return resources;
}

Resource * MeshSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	// If the type field is missing, then it cannot be loaded
	if (!resourceNode["type"])
	{
		printf("The node with ID of %s could not be loaded because it did not have a type field defined to it.\n", resourceNode["id"].as<std::string>().c_str());
		return nullptr;
	}

	// Read the type of the resource
	std::string type = resourceNode["type"].as<std::string>();

	// Deserialize to a specific resource type based on the type name
	if (type == "Mesh")
	{
		// If it is a mesh, make a mesh resource
		MeshResource* meshResource = new MeshResource();
		meshResource->setMeshIndex(resourceNode["index"].as<unsigned int>());
		meshResource->setMeshName(resourceNode["name"].as<std::string>());
		meshResource->setType(MeshResourceType);
		return meshResource;
	}

	// If we are here, then the type was not defined and we have no way to convert it into a resource type
	return nullptr;
}

bool MeshSerializer::findMeshResourceNode(YAML::Node root, unsigned int index, aiString name, YAML::Node& resourceNode)
{
	for (unsigned int i = 0; i < root.size(); i++)
	{
		YAML::Node resource = root[i];

		// Skip over any resource that is not a mesh
		std::string type = resource["type"].as<std::string>();
		if (type != "Mesh")
		{
			continue;
		}

		// If the mesh's name matches the supplied name, return the node
		std::string nodeName = resource["name"].as<std::string>();
		if (nodeName.c_str() == name.C_Str())
		{
			resourceNode = resource;
			return true;
		}

		// If the mesh's index matches the supplied index, return the node
		unsigned int nodeIndex = resource["index"].as<unsigned int>();
		if (nodeIndex == index)
		{
			resourceNode = resource;
			return true;
		}
	}

	return false;
}
