#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\GaussianBlurFilter.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class GaussianBlurFilterInspector : public InspectorRenderer, RegisterInspectorRenderer<GaussianBlurFilterInspector, Ext::GaussianBlurFilter>
	{
		REGISTER_INSPECTOR(GaussianBlurFilterInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
