#include "Files\Serializers\MeshFileSerializer.h"

#include <assimp/Importer.hpp>
#include "Helpers\AssimpUtils.h"
#include "Mesh.h"
#include "MeshResource.h"
#include "SkeletonResource.h"
#include "AnimationResource.h"

namespace DerydocaEngine::Files::Serializers {

	static Rendering::MeshFlags stringToFlag(std::string const& flagString)
	{
		if (flagString == "load_adjacent")
		{
			return Rendering::MeshFlags::load_adjacent;
		}

		return (Rendering::MeshFlags)0;
	}

	Files::FileType MeshSerializer::getFileType()
	{
		return Files::FileType::MeshFileType;
	}

	YAML::Node MeshSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		std::map<std::string, std::shared_ptr<Animation::Skeleton>> skeletons;

		std::map<std::string, boost::uuids::uuid> skeletonIdMap;

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

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

			if (mesh->HasBones())
			{
				std::shared_ptr<Animation::Skeleton> skeleton = Helpers::AssimpUtils::getSkeleton(scene, i);

				if (skeletons.find(skeleton->getName()) == skeletons.end())
				{
					skeletons.emplace(skeleton->getName(), skeleton);
					skeletonIdMap.emplace(skeleton->getName(), generateUuid());
				}
				resourceNode["Skeleton"] = skeletonIdMap[skeleton->getName()];
			}

			// Add it to the resource node
			resources.push_back(resourceNode);
		}

		for each (auto skeleton in skeletons)
		{
			YAML::Node skeletonResourceNode;
			skeletonResourceNode["ID"] = skeletonIdMap[skeleton.second->getName()];
			skeletonResourceNode["Type"] = "Skeleton";
			skeletonResourceNode["Name"] = skeleton.second->getName();
		
			resources.push_back(skeletonResourceNode);
		}

		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			YAML::Node animationResourceNode;
			animationResourceNode["ID"] = generateUuid();
			animationResourceNode["Type"] = "Animation";
			animationResourceNode["Name"] = std::string(scene->mAnimations[i]->mName.data);

			resources.push_back(animationResourceNode);
		}
		
		return resources;
	}

	Resources::Resource * MeshSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
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
			Resources::MeshResource* meshResource = new Resources::MeshResource();
			meshResource->setMeshIndex(resourceNode["Index"].as<unsigned int>());
			meshResource->setMeshName(resourceNode["Name"].as<std::string>());
			YAML::Node skeletonNode = resourceNode["Skeleton"];
			if (skeletonNode)
			{
				meshResource->setSkeletonId(skeletonNode.as<boost::uuids::uuid>());
			}
			meshResource->setType(Resources::MeshResourceType);

			YAML::Node flagsNode = resourceNode["Flags"];
			if (flagsNode && flagsNode.IsSequence())
			{
				for (size_t i = 0; i < flagsNode.size(); i++)
				{
					meshResource->setFlag(stringToFlag(flagsNode[0].as<std::string>()));
				}
			}

			return meshResource;
		}
		else if (type == "Skeleton")
		{
			Resources::SkeletonResource* skeletonResource = new Resources::SkeletonResource();
			skeletonResource->setName(resourceNode["Name"].as<std::string>());
			skeletonResource->setType(Resources::SkeletonResourceType);

			return skeletonResource;
		}
		else if (type == "Animation")
		{
			Resources::AnimationResource* animationResource = new Resources::AnimationResource();
			animationResource->setName(resourceNode["Name"].as<std::string>());
			animationResource->setType(Resources::AnimationResourceType);

			return animationResource;
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

}
