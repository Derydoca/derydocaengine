#include "SkeletonResourceSerializer.h"

#include "assimp\Importer.hpp"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "AssimpUtils.h"
#include "Skeleton.h"
#include "SkeletonResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	SkeletonResourceSerializer::SkeletonResourceSerializer()
	{
	}

	SkeletonResourceSerializer::~SkeletonResourceSerializer()
	{
	}

	void * SkeletonResourceSerializer::deserialize(Resource * const& resource)
	{
		return nullptr;
	}

	std::shared_ptr<void> SkeletonResourceSerializer::deserializePointer(Resource * const & resource)
	{
		SkeletonResource* sr = (SkeletonResource*)resource;
		Assimp::Importer importer;
		auto scene = importer.ReadFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		return Helpers::AssimpUtils::getSkeleton(scene, 0);
	}

	ResourceType SkeletonResourceSerializer::getResourceType()
	{
		return ResourceType::SkeletonResourceType;
	}

}
