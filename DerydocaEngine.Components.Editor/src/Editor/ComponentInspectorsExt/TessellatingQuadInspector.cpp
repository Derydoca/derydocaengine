#include "EditorComponentsPch.h"
#include "TessellatingQuadInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::TessellatingQuadInspector::render(std::shared_ptr<Object> object)
{
	auto quad = std::static_pointer_cast<DerydocaEngine::Ext::TessellatingQuad>(object);

	ImGui::InputInt("Inner", &quad->getInner());
	ImGui::InputInt("Outer", &quad->getOuter());
}
