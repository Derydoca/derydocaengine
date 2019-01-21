#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\MultisampleSwitch.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class MultisampleSwitchInspector : public InspectorRenderer, RegisterInspectorRenderer<MultisampleSwitchInspector, Ext::MultisampleSwitch>
	{
		REGISTER_INSPECTOR(MultisampleSwitchInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
