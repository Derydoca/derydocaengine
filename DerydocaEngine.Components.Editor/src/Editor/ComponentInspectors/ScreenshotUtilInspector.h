#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\ScreenshotUtil.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ScreenshotUtilInspector : public InspectorRenderer, RegisterInspectorRenderer<ScreenshotUtilInspector, Components::ScreenshotUtil>
	{
		REGISTER_INSPECTOR(ScreenshotUtilInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
