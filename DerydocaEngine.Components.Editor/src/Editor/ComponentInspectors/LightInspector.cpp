#include "EditorComponentsPch.h"
#include "LightInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::LightInspector::render(std::shared_ptr<Object> object)
{
	auto light = std::static_pointer_cast<Components::Light>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	ImGui::ColorEdit4("Color", &light->getColor().r);
	if (light->isCastingShadows())
	{
		ImGui::SliderFloat("Shadow Softness", &light->getShadowSoftness(), 0.0f, 0.03f);
		Dgui::InputProjection("Shadow Projection", light->getProjection());
	}

}
