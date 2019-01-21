#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\GammaCorrectionFilter.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class GammaCorrectionFilterInspector : public InspectorRenderer, RegisterInspectorRenderer<GammaCorrectionFilterInspector, Ext::GammaCorrectionFilter>
	{
		REGISTER_INSPECTOR(GammaCorrectionFilterInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
