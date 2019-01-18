#include "EditorComponentsPch.h"
#include "KeyboardMoverInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::KeyboardMoverInspector::render(std::shared_ptr<Object> object)
{
	ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, "This component is intended to be used exclusively for the editor.");
}
