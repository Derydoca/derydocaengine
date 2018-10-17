#pragma once
#include <vector>
#include "AnimationChannel.h"

namespace DerydocaEngine::Animation {

	class Keyframe {
	public:
		Keyframe();
		Keyframe(float timestamp, std::vector<AnimationChannel> channels);

		float getTimestamp() const { return m_timestamp; }
		AnimationChannel& getChannel(size_t channelIndex) { return m_channels[channelIndex]; }
	private:
		float m_timestamp;
		std::vector<AnimationChannel> m_channels;
	};

}
