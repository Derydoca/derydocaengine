#include "EditorComponentsPch.h"
#include "TerrainInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::TerrainInspector::render(std::shared_ptr<Object> object)
{
	auto terrain = std::static_pointer_cast<Components::Terrain>(object);

	ImGui::TextWrapped("The terrain component is currently broken. This will be addressed in a future update.");
}
