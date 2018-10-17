#include "Keyframe.h"

namespace DerydocaEngine::Animation {
	Keyframe::Keyframe() :
		m_timestamp(),
		m_channels()
	{
	}

	Keyframe::Keyframe(float timestamp, std::vector<AnimationChannel> channels) :
		m_timestamp(timestamp),
		m_channels(channels)
	{
	}

}
