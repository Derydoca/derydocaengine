#include "EditorPch.h"
#include "InputQuaternion.h"

namespace DerydocaEngine::Dgui
{

	bool InputQuaternion(
		const std::string& label,
		glm::quat& value,
		const char *format,
		ImGuiInputTextFlags extraFlags
	)
	{
		glm::vec3 eulerValues = glm::degrees(glm::eulerAngles(value));
		bool modified = ImGui::InputFloat3(label.c_str(), &eulerValues[0], format, extraFlags);
		if (modified)
		{
			value = glm::quat(glm::radians(eulerValues));
		}
		return modified;
	}

}
