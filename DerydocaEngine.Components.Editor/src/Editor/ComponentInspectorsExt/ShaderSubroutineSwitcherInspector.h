#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ShaderSubroutineSwitcher.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ShaderSubroutineSwitcherInspector : public InspectorRenderer, RegisterInspectorRenderer<ShaderSubroutineSwitcherInspector, Ext::ShaderSubroutineSwitcher>
	{
		REGISTER_INSPECTOR(ShaderSubroutineSwitcherInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
