#include "EditorComponentsPch.h"
#include "SkinnedMeshRendererInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::SkinnedMeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto skinnedMeshRenderer = std::static_pointer_cast<Components::SkinnedMeshRenderer>(object);

	ImGui::Text("There are currently no editable fields. A more comprehensive inspector will come in the future.");

}
