#include "EditorComponentsPch.h"
#include "EdgeDetectionFilterInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::EdgeDetectionFilterInspector::render(std::shared_ptr<Object> object)
{
	auto edgeDetectionFilter = std::static_pointer_cast<Ext::EdgeDetectionFilter>(object);

	ImGui::SliderFloat("Threshold", &edgeDetectionFilter->getEdgeThreshold(), 0.0f, 1.0f);
}
