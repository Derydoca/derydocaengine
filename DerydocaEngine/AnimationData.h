#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include "AnimationChannel.h"
#include "Bone.h"
#include "Skeleton.h"

namespace DerydocaEngine::Animation {

	class AnimationData {
	public:
		AnimationData();
		AnimationData(const std::string& name, const double duration, const std::vector<AnimationChannel>& channels);

		double getDuration() const { return m_duration; }
		std::string getName() const { return m_name; }
		const unsigned int getBoneId(std::string boneName);
		const AnimationChannel& getChannel(unsigned int boneId);
		const AnimationChannel& getChannel(std::string boneName);

	private:
		std::string m_name;
		double m_duration;
		std::vector<AnimationChannel> m_channels;

	};

}