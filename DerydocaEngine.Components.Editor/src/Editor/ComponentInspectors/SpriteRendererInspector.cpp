#include "EditorComponentsPch.h"
#include "SpriteRendererInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::SpriteRendererInspector::render(std::shared_ptr<Object> object)
{
	auto SpriteRenderer = std::static_pointer_cast<Components::SpriteRenderer>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	Color color = SpriteRenderer->getColor();
	if (ImGui::ColorEdit4("Color", &color.r))
	{
		SpriteRenderer->setColor(color);
	}

}
