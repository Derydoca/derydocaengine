#include "EnginePch.h"
#include "Animation\AnimationData.h"

#include <algorithm>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace DerydocaEngine::Animation {
	AnimationData::AnimationData() :
		m_name(),
		m_duration(),
		m_channels()
	{
	}

	AnimationData::AnimationData(const std::string& name, const double duration, const std::vector<AnimationChannel>& channels) :
		m_name(name),
		m_duration(duration),
		m_channels(channels)
	{
	}

	const unsigned int AnimationData::getBoneId(const std::string& boneName)
	{
		// Return the index of the first channel that has a matching name
		for (size_t i = 0; i < m_channels.size(); i++)
		{
			if (m_channels[i].boneName == boneName)
			{
				return (unsigned int)i;
			}
		}

		// Return an invalid index if no channel was found
		return (unsigned int)-1;
	}

	const AnimationChannel* AnimationData::getChannel(unsigned int boneId)
	{
		return &m_channels[boneId];
	}

	const AnimationChannel* AnimationData::getChannel(const std::string & boneName)
	{
		// Get the channel ID
		unsigned int boneId = getBoneId(boneName);
		assert(boneId != (unsigned int)-1);

		// And return the channel
		return getChannel(boneId);
	}

	void AnimationData::loadPose(float time, std::vector<glm::mat4>& boneTransforms, const std::shared_ptr<Skeleton>& skeleton)
	{
		// Convert the animation time into the range of the animation duration
		float animationTime = (float)fmod(time, m_duration);

		// Recursively set the bone positions
		const glm::mat4 parentTransform;
		loadPose(animationTime, boneTransforms, skeleton->getGlobalInverseTransform(), skeleton->getRootBone(), parentTransform);
	}

	void AnimationData::loadPose(float animationTime, std::vector<glm::mat4>& boneTransforms, const glm::mat4 & globalInverseTransform, const std::shared_ptr<Animation::Bone> bone, const glm::mat4 & parentTransform)
	{
		// Calculate the transformation of the current bone
		glm::mat4 boneTransform = bone->getOffset();
		const AnimationChannel* channel = getChannel(bone->getID());
		if (channel)
		{
			glm::vec3 translation;
			calcInterpolatedTranslation(translation, animationTime, channel);

			glm::quat rotation;
			calcInterpolatedRotation(rotation, animationTime, channel);

			glm::vec3 scale;
			calcInterpolatedScale(scale, animationTime, channel);

			boneTransform = glm::translate(translation) * glm::toMat4(rotation) * glm::scale(scale);
		}

		// Combine the bone's transform with all parent transforms that have come before it
		glm::mat4 globalTransformation = parentTransform * boneTransform;

		// Store the bone transform information
		boneTransforms[bone->getID()] = globalInverseTransform * globalTransformation * bone->getOffset();

		// Recursively load all child bones
		for (size_t i = 0; i < bone->getNumChildren(); i++)
		{
			//glm::mat4 globalTransformation;
			loadPose(animationTime, boneTransforms, globalInverseTransform, bone->getChildBone((unsigned int)i), globalTransformation);
		}
	}

	void AnimationData::calcInterpolatedScale(glm::vec3 & scale, const float animationTime, const AnimationChannel* channel)
	{
		switch (channel->scaleKeys.size())
		{
		case 0:
			scale = glm::vec3(1.0f);
			break;
		case 1:
			scale = channel->scaleKeys[0].value;
			break;
		default:
			// Get the indices surrounding the current playhead
			size_t scalingIndex = getPrevIndex(channel->scaleKeys, animationTime);
			size_t nextScalingIndex = scalingIndex + 1;
			assert(nextScalingIndex < channel->scaleKeys.size());
			
			// Get the amount of time that exists between the two different animation keys
			float deltaTime = (float)(channel->scaleKeys[nextScalingIndex].time - channel->scaleKeys[scalingIndex].time);

			// Get a normalized percentage of the current playhead between the two nearest keys
			float factor = (animationTime - (float)channel->scaleKeys[scalingIndex].time) / deltaTime;
			assert(factor >= 0.0f && factor <= 1.0f);

			// Calculate the final value
			const glm::vec3 start = channel->scaleKeys[scalingIndex].value;
			const glm::vec3 end = channel->scaleKeys[nextScalingIndex].value;
			glm::vec3 delta = end - start;
			scale = start + factor * delta;
			break;
		}
	}

	void AnimationData::calcInterpolatedRotation(glm::quat & rotation, const float animationTime, const AnimationChannel* channel)
	{
		switch (channel->rotationKeys.size())
		{
		case 0:
			rotation = glm::quat();
			break;
		case 1:
			rotation = channel->rotationKeys[0].value;
			break;
		default:
			// Get the indices surrounding the current playhead
			size_t prevIndex = getPrevIndex(channel->rotationKeys, animationTime);
			size_t nextIndex = prevIndex + 1;
			assert(nextIndex < channel->rotationKeys.size());

			// Get the amount of time that exists between the two different animation keys
			float deltaTime = (float)(channel->rotationKeys[nextIndex].time - channel->rotationKeys[prevIndex].time);

			// Get a normalized percentage of the current playhead between the two nearest keys
			float factor = (animationTime - (float)channel->rotationKeys[prevIndex].time) / deltaTime;
			assert(factor >= 0.0f && factor <= 1.0f);

			// Calculate the final value
			const glm::quat start = channel->rotationKeys[prevIndex].value;
			const glm::quat end = channel->rotationKeys[nextIndex].value;
			rotation = glm::slerp(start, end, factor);
			break;
		}
	}

	void AnimationData::calcInterpolatedTranslation(glm::vec3 & position, const float animationTime, const AnimationChannel* channel)
	{
		switch (channel->positionKeys.size())
		{
		case 0:
			position = glm::vec3();
			break;
		case 1:
			position = channel->positionKeys[0].value;
			break;
		default:
			// Get the indices surrounding the current playhead
			size_t prevIndex = getPrevIndex(channel->positionKeys, animationTime);
			size_t nextIndex = prevIndex + 1;
			assert(nextIndex < channel->positionKeys.size());

			// Get the amount of time that exists between the two different animation keys
			float deltaTime = (float)(channel->positionKeys[nextIndex].time - channel->positionKeys[prevIndex].time);

			// Get a normalized percentage of the current playhead between the two nearest keys
			float factor = (animationTime - (float)channel->positionKeys[prevIndex].time) / deltaTime;
			assert(factor >= 0.0f && factor <= 1.0f);

			// Calculate the final value
			const glm::vec3 start = channel->positionKeys[prevIndex].value;
			const glm::vec3 end = channel->positionKeys[nextIndex].value;
			glm::vec3 delta = end - start;
			position = start + factor * delta;
			break;
		}
	}

	void AnimationData::optimizeForSkeleton(const std::shared_ptr<Skeleton>& skeleton)
	{
		const std::shared_ptr<Bone> rootBone = skeleton->getRootBone();
		optimizeForSkeleton(rootBone);

		std::sort(m_channels.begin(), m_channels.end());
	}

	void AnimationData::optimizeForSkeleton(const std::shared_ptr<Bone> bone)
	{
		for (size_t i = 0; i < m_channels.size(); i++)
		{
			if (m_channels[i].boneName == bone->getName())
			{
				m_channels[i].id = bone->getID();
			}
		}

		for (unsigned int i = 0; i < bone->getNumChildren(); i++)
		{
			optimizeForSkeleton(bone->getChildBone(i));
		}
	}

}
