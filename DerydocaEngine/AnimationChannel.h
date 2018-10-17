#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace DerydocaEngine::Animation {

	struct AnimationChannel
	{
		glm::vec3 Translation;
		glm::quat Rotation;
		glm::vec3 Scale;
	};

}
