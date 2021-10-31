#include "EditorComponentsPch.h"
#include "LightInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::LightInspector::render(std::shared_ptr<Object> object)
{
	auto light = std::static_pointer_cast<Components::Light>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	ImGui::ColorEdit4("Color", &light->getColor().r);

}
