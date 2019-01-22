#include "EditorComponentsPch.h"
#include "ShaderSubroutineSwitcherInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ShaderSubroutineSwitcherInspector::render(std::shared_ptr<Object> object)
{
	auto switcher = std::static_pointer_cast<Ext::ShaderSubroutineSwitcher>(object);
	ImGui::TextWrapped("There are no editable fields.");
}
