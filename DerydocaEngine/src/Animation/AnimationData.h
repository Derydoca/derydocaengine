#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include "Animation\AnimationChannel.h"
#include "Animation\Skeleton.h"

namespace DerydocaEngine::Animation {

	class AnimationData {
	public:
		AnimationData();
		AnimationData(const std::string& name, const double duration, const std::vector<AnimationChannel>& channels);

		double getDuration() const { return m_duration; }
		std::string getName() const { return m_name; }
		const unsigned int getBoneId(const std::string & boneName);
		const AnimationChannel* getChannel(unsigned int boneId);
		const AnimationChannel* getChannel(const std::string & boneName);
		void loadPose(float time, std::vector<glm::mat4>& boneTransforms, const std::shared_ptr<Skeleton>& skeleton);
		void optimizeForSkeleton(const std::shared_ptr<Skeleton>& skeleton);

	private:
		std::string m_name;
		double m_duration;
		std::vector<AnimationChannel> m_channels;

		void loadPose(float animationTime, std::vector<glm::mat4>& boneTransforms, const glm::mat4& globalInverseTransform, const std::shared_ptr<Animation::Bone> bone, const glm::mat4& parentTransform);
		void calcInterpolatedScale(glm::vec3& scale, const float animationTime, const AnimationChannel* channel);
		void calcInterpolatedRotation(glm::quat & rotation, const float animationTime, const AnimationChannel* channel);
		void calcInterpolatedTranslation(glm::vec3 & position, const float animationTime, const AnimationChannel* channel);
		void optimizeForSkeleton(const std::shared_ptr<Bone> bone);

		template <typename T>
		size_t getPrevIndex(const std::vector<AnimationKey<T>> & animationKeys, float animationTime)
		{
			for (size_t i = 0; i < animationKeys.size() - 1; i++)
			{
				if (animationTime < animationKeys[i + 1].time)
				{
					return i;
				}
			}

			return 0;
		}
	};

}