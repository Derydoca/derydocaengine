#include "EditorComponentsPch.h"
#include "ScreenshotUtilInspector.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::ScreenshotUtilInspector::render(std::shared_ptr<Object> object)
{
	auto screenshotUtil = std::static_pointer_cast<Components::ScreenshotUtil>(object);

	if (ImGui::Button("Take Screenshot"))
	{
		screenshotUtil->saveScreenshot();
	}
}
