#pragma once

namespace DerydocaEngine::Dgui
{

	bool InputQuaternion(
		const std::string& label,
		glm::quat& value,
		const char *format = "%.3f",
		ImGuiInputTextFlags extraFlags = 0
	);

}