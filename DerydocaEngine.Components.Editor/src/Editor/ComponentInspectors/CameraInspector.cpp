#include "EditorComponentsPch.h"
#include "CameraInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::CameraInspector::render(std::shared_ptr<Object> object)
{
	auto camera = std::static_pointer_cast<Components::Camera>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	Dgui::InputProjection("Projection", camera->getProjection());

	// Display a combo box for selecting colors
	// TODO: Automate the creation of combo boxes
	std::string clearModeText;
	switch (camera->getClearMode())
	{
	case Components::Camera::ClearMode::ColorClear:
		clearModeText = "Color";
		break;
	case Components::Camera::ClearMode::SkyboxClear:
		clearModeText = "Skybox";
		break;
	case Components::Camera::ClearMode::NoClear:
		clearModeText = "No Clear";
		break;
	default:
		clearModeText = "[Unknown]";
		break;
	}
	if (ImGui::BeginCombo("Clear Mode", clearModeText.c_str()))
	{
		if (ImGui::Selectable("Color"))
		{
			camera->setClearMode(Components::Camera::ClearMode::ColorClear);
		}
		if (ImGui::Selectable("Skybox"))
		{
			camera->setClearMode(Components::Camera::ClearMode::SkyboxClear);
		}
		if (ImGui::Selectable("No Clear"))
		{
			camera->setClearMode(Components::Camera::ClearMode::NoClear);
		}
		ImGui::EndCombo();
	}

	if (camera->getClearMode() == Components::Camera::ClearMode::ColorClear)
	{
		ImGui::ColorEdit3("Clear Color", &camera->getClearColor().r);
	}
}
