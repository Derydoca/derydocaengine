#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "AnimationKey.h"

namespace DerydocaEngine::Animation {

	struct AnimationChannel
	{
		AnimationChannel()
		{
		}

		AnimationChannel(
			std::string boneName,
			std::vector<AnimationKey<glm::vec3>> positionKeys,
			std::vector<AnimationKey<glm::quat>> rotationKeys,
			std::vector<AnimationKey<glm::vec3>> scaleKeys) :
			boneName(boneName),
			positionKeys(positionKeys),
			rotationKeys(rotationKeys),
			scaleKeys(scaleKeys)
		{
		}

		unsigned int id;
		std::string boneName;
		std::vector<AnimationKey<glm::vec3>> positionKeys;
		std::vector<AnimationKey<glm::quat>> rotationKeys;
		std::vector<AnimationKey<glm::vec3>> scaleKeys;

		bool operator>(const AnimationChannel & other)
		{
			return id > other.id;
		}
		bool operator<(const AnimationChannel & other)
		{
			return id < other.id;
		}
	};

}
