#pragma once

namespace DerydocaEngine::Dgui
{

	bool InputQuaternion(
		const std::string& label,
		glm::quat& value,
		float speed = 1.0f,
		float min = 0.0f,
		float max = 0.0f,
		const char *format = "%.3f",
		float power = 1.0f
	);

}