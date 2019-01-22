#include "EditorComponentsPch.h"
#include "ParticleSystemInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ParticleSystemInspector::render(std::shared_ptr<Object> object)
{
	auto particleSystem = std::static_pointer_cast<Ext::ParticleSystem>(object);

	ImGui::InputInt("Particle Count", &particleSystem->getNumParticles());
	ImGui::InputFloat("Particle Size", &particleSystem->getSize());
	ImGui::DragFloat3("Volume Size", &particleSystem->getVolumeSize()[0]);

	if (ImGui::Button("Reset"))
	{
		particleSystem->reset();
	}
}
