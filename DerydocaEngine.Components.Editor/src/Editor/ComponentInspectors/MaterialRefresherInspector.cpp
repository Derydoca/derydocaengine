#include "EditorComponentsPch.h"
#include "MaterialRefresherInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::MaterialRefresherInspector::render(std::shared_ptr<Object> object)
{
	auto materialRefresher = std::static_pointer_cast<Components::MaterialRefresher>(object);

	ImGui::TextWrapped("This will refresh a material attached to this object whenever the source files have been modified.");
}
