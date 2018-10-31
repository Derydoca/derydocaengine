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

		unsigned int animIndex = 0;
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			std::string animName = scene->mAnimations[i]->mName.data;
			if (animName == ar->getName())
			{
				animIndex = i;
				break;
			}
		}

		return Helpers::AssimpUtils::getAnimation(scene, animIndex);
	}

	ResourceType AnimationResourceSerializer::getResourceType()
	{
		return ResourceType::AnimationResourceType;
	}

}
