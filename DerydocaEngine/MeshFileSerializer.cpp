#include "MeshFileSerializer.h"
#include "MeshResource.h"
#include "Mesh.h"
#include <iostream>

using namespace std;

static MeshFlags stringToFlag(string const& flagString)
{
	if (flagString == "load_adjacent")
	{
		return MeshFlags::load_adjacent;
	}

	return (MeshFlags)0;
}

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

YAML::Node MeshSerializer::generateResourceNodes(std::string const& filePath)
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
			resourceNode["ID"] = generateUuid();
			resourceNode["Type"] = "Mesh";
		}

		// Set object properties
		resourceNode["Index"] = i;
		resourceNode["Name"] = mesh->mName.C_Str();

		// Add it to the resource node
		resources.push_back(resourceNode);
	}

	return resources;
}

Resource * MeshSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
{
	// If the type field is missing, then it cannot be loaded
	if (!resourceNode["Type"])
	{
		printf("The node with ID of %s could not be loaded because it did not have a type field defined to it.\n", resourceNode["id"].as<std::string>().c_str());
		return nullptr;
	}

	// Read the type of the resource
	std::string type = resourceNode["Type"].as<std::string>();

	// Deserialize to a specific resource type based on the type name
	if (type == "Mesh")
	{
		// If it is a mesh, make a mesh resource
		MeshResource* meshResource = new MeshResource();
		meshResource->setMeshIndex(resourceNode["Index"].as<unsigned int>());
		meshResource->setMeshName(resourceNode["Name"].as<std::string>());
		meshResource->setType(MeshResourceType);
		
		YAML::Node flagsNode = resourceNode["Flags"];
		if (flagsNode && flagsNode.IsSequence())
		{
			for (size_t i = 0; i < flagsNode.size(); i++)
			{
				meshResource->setFlag(stringToFlag(flagsNode[0].as<string>()));
			}
		}

		return meshResource;
	}

	// If we are here, then the type was not defined and we have no way to convert it into a resource type
	return nullptr;
}

bool MeshSerializer::findMeshResourceNode(YAML::Node const& root, unsigned int const& index, aiString const& name, YAML::Node & resourceNode)
{
	for (unsigned int i = 0; i < root.size(); i++)
	{
		YAML::Node resource = root[i];

		// Skip over any resource that is not a mesh
		std::string type = resource["Type"].as<std::string>();
		if (type != "Mesh")
		{
			continue;
		}

		// If the mesh's name matches the supplied name, return the node
		std::string nodeName = resource["Name"].as<std::string>();
		if (nodeName.c_str() == name.C_Str())
		{
			resourceNode = resource;
			return true;
		}

		// If the mesh's index matches the supplied index, return the node
		unsigned int nodeIndex = resource["Index"].as<unsigned int>();
		if (nodeIndex == index)
		{
			resourceNode = resource;
			return true;
		}
	}

	return false;
}
