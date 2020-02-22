#include "EnginePch.h"
#include "Files\Serializers\MeshFileSerializer.h"

#include <assimp/Importer.hpp>
#include "Helpers\AssimpUtils.h"
#include "Rendering\Mesh.h"
#include "Resources\MeshResource.h"
#include "Resources\SkeletonResource.h"
#include "Resources\AnimationResource.h"

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

	std::vector<std::shared_ptr<Resources::Resource>> MeshSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(assetPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		std::map<std::string, std::shared_ptr<Animation::Skeleton>> skeletons;
		std::map<std::string, boost::uuids::uuid> skeletonIdMap;

		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			/* The legacy code previously only generated an ID/Type value if there was another node already
			created in the list that had a name matching this current name (mesh->mName). I've omitted it
			since it is unclear if it is necessary*/
			auto resource = std::make_shared<Resources::MeshResource>();
			resource->generateAndSetId();
			// TODO: Set the hard-coded value Type to "Mesh"
			resource->setMeshIndex(static_cast<const unsigned int>(i));
			resource->setMeshName(mesh->mName.data);

			if (mesh->HasBones())
			{
				std::shared_ptr<Animation::Skeleton> skeleton = Helpers::AssimpUtils::getSkeleton(scene, static_cast<unsigned int>(i));

				if (skeletons.find(skeleton->getName()) == skeletons.end())
				{
					skeletons.emplace(skeleton->getName(), skeleton);
					skeletonIdMap.emplace(skeleton->getName(), generateUuid());
				}
				resource->setSkeletonId(skeletonIdMap[skeleton->getName()]);
			}

			resources.push_back(resource);
		}

		for each (auto skeleton in skeletons)
		{
			auto skeletonResource = std::make_shared<Resources::SkeletonResource>();
			skeletonResource->setId(skeletonIdMap[skeleton.second->getName()]);
			// TODO: Set the hard-coded value Type to "Skeleton"
			skeletonResource->setName(skeleton.second->getName());

			resources.push_back(skeletonResource);
		}

		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			auto animationResource = std::make_shared<Resources::AnimationResource>();
			animationResource->generateAndSetId();
			// TODO: Set the hard-coded value Type to "Animation"
			animationResource->setName(scene->mAnimations[i]->mName.data);
			
			resources.push_back(animationResource);
		}

		return resources;
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
