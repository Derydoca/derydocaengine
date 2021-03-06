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

}
