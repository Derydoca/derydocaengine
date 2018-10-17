#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include "Bone.h"
#include "Keyframe.h"
#include "Skeleton.h"

namespace DerydocaEngine::Animation {

	class AnimationData {
	public:
		AnimationData();
		AnimationData(const std::string& name, const float duration, const std::vector<Keyframe>& keyframes);

		float getDuration() const { return m_duration; }
		std::string getName() const { return m_name; }
		void populateSkeletonTransforms(float time, const Skeleton& skeleton, std::vector<glm::mat4>& skeletonTransforms);

	private:
		std::string m_name;
		float m_duration;
		std::vector<Keyframe> m_keyframes;

		size_t getPriorKeyframeIndex(float animationTime) const;
		void loadSkeletonTransforms(float animationTime, const Bone& bone, const glm::mat4& parentTransform, std::vector<glm::mat4>& skeletonTransforms) const;
	};

}