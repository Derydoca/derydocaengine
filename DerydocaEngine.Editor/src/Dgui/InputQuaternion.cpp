#include "EditorPch.h"
#include "InputQuaternion.h"

namespace DerydocaEngine::Dgui
{

	bool InputQuaternion(
		const std::string& label,
		glm::quat& value,
		float speed,
		float min,
		float max,
		const char *format,
		float power
	)
	{
		glm::vec3 eulerValues = glm::degrees(glm::eulerAngles(value));
		bool modified = ImGui::DragFloat3(
			label.c_str(),
			&eulerValues[0],
			speed,
			min,
			max,
			format,
			power
		);
		if (modified)
		{
			value = glm::quat(glm::radians(eulerValues));
		}
		return modified;
	}

}
