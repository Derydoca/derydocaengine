#include "EditorComponentsPch.h"
#include "BezierCurveRendererInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::BezierCurveRendererInspector::render(std::shared_ptr<Object> object)
{
	auto bezierCurveRenderer = std::static_pointer_cast<Ext::BezierCurveRenderer>(object);
	ImGui::ColorEdit3("Color", &bezierCurveRenderer->getColor().r);
	ImGui::SliderInt("Segments", &bezierCurveRenderer->getNumSegments(), 1, 32);
}
