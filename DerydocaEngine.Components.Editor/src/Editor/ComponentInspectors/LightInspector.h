#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\Light.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class LightInspector : public InspectorRenderer, RegisterInspectorRenderer<LightInspector, Components::Light>
	{
		REGISTER_INSPECTOR(LightInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
