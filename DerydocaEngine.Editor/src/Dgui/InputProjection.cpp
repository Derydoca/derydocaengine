#include "EditorPch.h"
#include "InputProjection.h"

namespace DerydocaEngine::Dgui
{

	bool InputProjection(
		const std::string& label,
		Rendering::Projection& projection
	)
	{
		bool updated = false;

		ImGui::Text(label.c_str());
		ImGui::TreePush();
		updated |= ImGui::SliderFloat("FOV", &projection.getFov(), 1.0f, 180.0f);
		updated |= ImGui::SliderFloat("Near Plane", &projection.getZNear(), 0.01, 100.0f);
		updated |= ImGui::SliderFloat("Far Plane", &projection.getZFar(), 0.01, 100.0f);
		ImGui::TreePop();

		if (updated)
		{
			projection.recalculateProjectionMatrix();
		}

		return updated;
	}

}