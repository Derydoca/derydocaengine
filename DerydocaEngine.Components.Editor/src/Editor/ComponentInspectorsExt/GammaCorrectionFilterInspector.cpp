#include "EditorComponentsPch.h"
#include "GammaCorrectionFilterInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::GammaCorrectionFilterInspector::render(std::shared_ptr<Object> object)
{
	auto gammaCorrectionFilter = std::static_pointer_cast<Ext::GammaCorrectionFilter>(object);

	ImGui::DragFloat("Gamma", &gammaCorrectionFilter->getGamma(), 0.0f, 3.0f);
}
