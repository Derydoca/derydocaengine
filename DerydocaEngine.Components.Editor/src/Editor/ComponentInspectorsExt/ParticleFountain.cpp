#include "EditorComponentsPch.h"
#include "ParticleFountain.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ParticleFountainInspector::render(std::shared_ptr<Object> object)
{
	auto particleFountain = std::static_pointer_cast<Ext::ParticleFountain>(object);

	ImGui::TextWrapped("This editor is buggy. This component was not originally created to be updated at runtime but feel free to modify whatever values you like and play around!");

	ImGui::DragFloat("Min Velocity", &particleFountain->getVelocityMin());
	ImGui::DragFloat("Max Velocity", &particleFountain->getVelocityMax());
	ImGui::InputInt("Particle Count", &particleFountain->getNumParticles());
	ImGui::InputFloat("Lifetime", &particleFountain->getLifetime());
	ImGui::SliderFloat("Spawn Angle", &particleFountain->getAngle(), 0.0f, 180.0f);

	if (ImGui::Button("Reset Simulation"))
	{
		particleFountain->resetSimulation();
	}
}
