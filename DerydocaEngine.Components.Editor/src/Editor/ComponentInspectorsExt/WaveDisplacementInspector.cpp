#include "EditorComponentsPch.h"
#include "WaveDisplacementInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::WaveDisplacementInspector::render(std::shared_ptr<Object> object)
{
	auto wave = std::static_pointer_cast<DerydocaEngine::Ext::WaveDisplacement>(object);

	ImGui::DragFloat("K", &wave->getK(), 0.05f);
	ImGui::DragFloat("Velocity", &wave->getVelocity(), 0.05f);
	ImGui::DragFloat("Amplitude", &wave->getAmplitude(), 0.05f);
}
