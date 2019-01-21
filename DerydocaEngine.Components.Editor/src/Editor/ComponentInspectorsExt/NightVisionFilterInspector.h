#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\NightVisionFilter.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class NightVisionFilterInspector : public InspectorRenderer, RegisterInspectorRenderer<NightVisionFilterInspector, Ext::NightVisionFilter>
	{
		REGISTER_INSPECTOR(NightVisionFilterInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
