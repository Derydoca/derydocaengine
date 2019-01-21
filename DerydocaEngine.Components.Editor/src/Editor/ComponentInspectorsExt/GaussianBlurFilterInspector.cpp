#include "EditorComponentsPch.h"
#include "GaussianBlurFilterInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::GaussianBlurFilterInspector::render(std::shared_ptr<Object> object)
{
	auto gaussianBlurFilter = std::static_pointer_cast<Ext::GaussianBlurFilter>(object);
	ImGui::TextWrapped("There are currently no editable fields.", object->getId());
}
