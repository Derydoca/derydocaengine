#include "EditorComponentsPch.h"
#include "CameraInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::CameraInspector::render(std::shared_ptr<Object> object)
{
	auto camera = std::static_pointer_cast<Components::Camera>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	Dgui::InputProjection("Projection", camera->getProjection());
	ImGui::ColorEdit3("Clear Color", &camera->getClearColor().r);
}
