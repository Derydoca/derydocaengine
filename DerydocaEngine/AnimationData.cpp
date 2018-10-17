#include "AnimationData.h"

#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/transform.hpp>

namespace DerydocaEngine::Animation {
	AnimationData::AnimationData() :
		m_name(),
		m_duration(),
		m_keyframes()
	{
	}

	AnimationData::AnimationData(const std::string& name, const float duration, const std::vector<Keyframe>& keyframes) :
		m_name(name),
		m_duration(duration),
		m_keyframes(keyframes)
	{
	}

	void AnimationData::populateSkeletonTransforms(float time, const Skeleton& skeleton, std::vector<glm::mat4>& skeletonTransforms)
	{
		float animationTime = fmod(time, m_duration);
		//...
	}

	size_t AnimationData::getPriorKeyframeIndex(float animationTime) const
	{
		// Return the index of the first keyframe that meets or exceedes
		for (size_t i = 0; i < m_keyframes.size() - 1; i++)
		{
			if (m_keyframes[i + 1].getTimestamp() >= animationTime)
			{
				return i;
			}
		}

		// If all else fails, return the first element index
		return 0;
	}

	void AnimationData::loadSkeletonTransforms(float animationTime, const Bone& bone, const glm::mat4& parentTransform, std::vector<glm::mat4>& skeletonTransforms) const
	{
		if (m_keyframes.size() == 1)
		{
			//...
		}

		// Figure out the index of the keyframe that is immediately before the animation time provided
		size_t prevKeyframeIndex = getPriorKeyframeIndex(animationTime);

		// Get previous and next keyframe data
		Keyframe prevKeyframe = m_keyframes[prevKeyframeIndex];
		Keyframe nextKeyframe = m_keyframes[prevKeyframeIndex + 1];
		AnimationChannel prevChannel = prevKeyframe.getChannel(bone.getID());
		AnimationChannel nextChannel = nextKeyframe.getChannel(bone.getID());

		// Calculate the normalized time we are between keyframes
		float timeAfterPrev = animationTime - prevKeyframe.getTimestamp();
		float spanBetweenKeyframes = nextKeyframe.getTimestamp() - prevKeyframe.getTimestamp();
		float mixAmount = spanBetweenKeyframes != 0 ? timeAfterPrev / spanBetweenKeyframes : 0.0f;

		// Mix the transform, rotation, and scale variables based on the mix amount
		glm::vec3 transform = glm::mix(prevChannel.Translation, nextChannel.Translation, mixAmount);
		glm::quat rotation = glm::mix(prevChannel.Rotation, nextChannel.Rotation, mixAmount);
		rotation = glm::normalize(rotation);
		glm::vec3 scale = glm::mix(prevChannel.Scale, nextChannel.Scale, mixAmount);

		// Build the matrix from the transform, rotation, and scale data
		glm::mat4 globalTransform = glm::translate(transform) * glm::mat4_cast(rotation) * glm::scale(scale);

		// Store it in the skeleton transforms array
		skeletonTransforms[bone.getID()] = globalTransform;

		// Iterate through all of the children recursively
		for (unsigned int i = 0; i < bone.getNumChildren(); i++)
		{
			//loadSkeletonTransforms(animationTime, &bone[i], globalTransform, skeletonTransforms);
		}
	}

}
