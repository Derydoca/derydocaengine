#include "EditorComponentsPch.h"
#include "ParticleContinuousFountainInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ParticleContinuousFountainInspector::render(std::shared_ptr<Object> object)
{
	auto particleFountain = std::static_pointer_cast<Ext::ParticleContinuousFountain>(object);

	ImGui::TextWrapped("This editor is buggy. This component was not originally created to be updated at runtime but feel free to modify whatever values you like and play around!");

	ImGui::DragFloat3("Min Velocity", &particleFountain->getVelocityMin()[0]);
	ImGui::DragFloat3("Max Velocity", &particleFountain->getVelocityMax()[0]);
	ImGui::InputInt("Particle Count", &particleFountain->getNumParticles());
	ImGui::InputFloat("Lifetime", &particleFountain->getLifetime());
	ImGui::SliderFloat("Spawn Angle", &particleFountain->getAngle(), 0.0f, 180.0f);
	ImGui::DragFloat3("Acceleration", &particleFountain->getAcceleration()[0], 0.0f, 10.0f);
	ImGui::InputFloat3("Emitter Size", &particleFountain->getEmitterSize()[0]);
	ImGui::DragFloatRange2("Particle Size", &particleFountain->getParticleSizeMin(), &particleFountain->getParticleSizeMax(), 1.0f, 0.0f, 100.0f);

	if (ImGui::Button("Reset Simulation"))
	{
		particleFountain->resetSimulation();
	}
}
