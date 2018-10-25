#include "AnimationResourceSerializer.h"

#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "AnimationData.h"
#include "AnimationResource.h"
#include "AssimpUtils.h"

namespace DerydocaEngine::Resources::Serializers
{

	AnimationResourceSerializer::AnimationResourceSerializer()
	{
	}

	AnimationResourceSerializer::~AnimationResourceSerializer()
	{
	}

	void * AnimationResourceSerializer::deserialize(Resource * const& resource)
	{
		return nullptr;
	}

	std::shared_ptr<void> AnimationResourceSerializer::deserializePointer(Resource * const & resource)
	{
		AnimationResource* ar = (AnimationResource*)resource;
		auto scene = aiImportFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		return Helpers::AssimpUtils::getAnimation(scene, 0);
	}

	ResourceType AnimationResourceSerializer::getResourceType()
	{
		return ResourceType::AnimationResourceType;
	}

}
