#include "EditorComponentsPch.h"
#include "MeshRendererInspector.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::MeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto meshRenderer = std::static_pointer_cast<Components::MeshRenderer>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	// TODO: Replace with a control that can set the actual material
	auto material = meshRenderer->getMaterial();
	ImGui::LabelText("Material", material == nullptr ? "[NOT SET]" : "[IS SET]");
	
	// TODO: Replace with a control that can set the actual mesh
	auto mesh = meshRenderer->getMesh();
	ImGui::LabelText("Mesh", mesh == nullptr ? "[NOT SET]" : "[IS SET]");

}
