#include "EditorComponentsPch.h"
#include "FrameStatsInspector.h"
#include "Components\TextRenderer.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::FrameStatsInspector::render(std::shared_ptr<Object> object)
{
	auto frameStats = std::static_pointer_cast<Components::FrameStats>(object);

	ImGui::Text("This component will update a TextRenderer component on the same GameObject with the current framerate.");

	std::weak_ptr<DerydocaEngine::Components::TextRenderer> textRendererComponent = frameStats->getComponentInChildren<DerydocaEngine::Components::TextRenderer>();
	if (!textRendererComponent.lock())
	{
		ImGui::Text("Please attach a TextRenderer component to this GameObject.");
	}
}
