#include "EditorComponentsPch.h"
#include "NoiseTextureInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::NoiseTextureInspector::render(std::shared_ptr<Object> object)
{
	auto noiseTexture = std::static_pointer_cast<Ext::NoiseTexture>(object);
	ImGui::InputInt("Width", &noiseTexture->getWidth(), 1, 5);
	ImGui::InputInt("Height", &noiseTexture->getHeight(), 1, 5);
	ImGui::SliderFloat("Base Frequency", &noiseTexture->getBaseFrequency(), 0.0f, 32.0f);
	ImGui::SliderFloat("Persistence", &noiseTexture->getPersistence(), 0.0f, 2.0f);
	ImGui::Checkbox("Is Seamless", &noiseTexture->getIsSeamless());
	ImGui::Checkbox("Is Periodic", &noiseTexture->getIsPeriodic());

	if (ImGui::Button("Regenerate"))
	{
		noiseTexture->generateNoiseTexture();
	}
}
