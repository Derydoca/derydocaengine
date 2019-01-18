#include "EditorComponentsPch.h"
#include "TextRendererInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::TextRendererInspector::render(std::shared_ptr<Object> object)
{
	auto textRenderer = std::static_pointer_cast<Components::TextRenderer>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	Color color = textRenderer->getColor();
	if (ImGui::ColorEdit4("Color", &color.r))
	{
		textRenderer->setColor(color);
	}

}
