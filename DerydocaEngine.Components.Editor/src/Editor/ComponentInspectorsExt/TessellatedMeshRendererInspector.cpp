#include "EditorComponentsPch.h"
#include "TessellatedMeshRendererInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::TessellatedMeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto tessMesh = std::static_pointer_cast<Ext::TessellatedMeshRenderer>(object);

	bool& useDynamicTess = tessMesh->getUseDynamicTessellation();

	ImGui::TextWrapped("This component is not actually reflecting the values entered here. More research is required on this component inspector.");

	ImGui::Checkbox("Dynamic Tessellation", &useDynamicTess);
	if (useDynamicTess)
	{
		ImGui::InputInt("Min Tessellation", &tessMesh->getMinDynamicTessLevel());
		ImGui::InputInt("Max Tessellation", &tessMesh->getMaxDynamicTessLevel());
		ImGui::InputFloat("Min Distance", &tessMesh->getMinDynamicTessDistance());
		ImGui::InputFloat("Max Distance", &tessMesh->getMaxDynamicTessDistance());
	}
	else
	{
		ImGui::InputInt("Tessellation Level", &tessMesh->getTessellationLevel());
	}
}
