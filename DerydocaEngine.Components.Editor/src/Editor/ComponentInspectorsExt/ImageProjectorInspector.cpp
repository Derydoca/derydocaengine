#include "EditorComponentsPch.h"
#include "ImageProjectorInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ImageProjectorInspector::render(std::shared_ptr<Object> object)
{
	auto imageProjector = std::static_pointer_cast<Ext::ImageProjector>(object);

	ImGui::SliderFloat("Field of View", &imageProjector->getFov(), 1.0f, 180.0f);
}
