#include "EditorComponentsPch.h"
#include "ParticleInstancedInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ParticleInstancedInspector::render(std::shared_ptr<Object> object)
{
	auto particleFountain = std::static_pointer_cast<Ext::ParticleInstanced>(object);

	ImGui::TextWrapped("This editor is buggy. This component was not originally created to be updated at runtime but feel free to modify whatever values you like and play around!");

	ImGui::DragFloat("Min Velocity", &particleFountain->getVelocityMin());
	ImGui::DragFloat("Max Velocity", &particleFountain->getVelocityMax());
	ImGui::InputInt("Particle Count", &particleFountain->getNumParticles());
	ImGui::InputFloat("Lifetime", &particleFountain->getLifetime());
	ImGui::SliderFloat("Spawn Angle", &particleFountain->getAngle(), 0.0f, 180.0f);
	ImGui::InputFloat3("Gravity", &particleFountain->getGravity()[0]);

	if (ImGui::Button("Reset Simulation"))
	{
		particleFountain->resetSimulation();
	}
}
