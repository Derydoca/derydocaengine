#include "EditorComponentsPch.h"
#include "WasdMoverInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::WasdMoverInspector::render(std::shared_ptr<Object> object)
{
	auto wasdMover = std::static_pointer_cast<Components::WasdMover>(object);

	ImGui::TextColored({1.0, 0.0, 0.0, 1.0}, "This component is intended to be used exclusively for the editor. %s", wasdMover->getClassName());
}
