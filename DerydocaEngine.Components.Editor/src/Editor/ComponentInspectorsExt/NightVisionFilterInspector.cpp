#include "EditorComponentsPch.h"
#include "NightVisionFilterInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::NightVisionFilterInspector::render(std::shared_ptr<Object> object)
{
	auto nightVisionFilter = std::static_pointer_cast<Ext::NightVisionFilter>(object);
	ImGui::SliderFloat("Radius", &nightVisionFilter->getRadius(), 1.0f, 1000.0f);
}
