#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\EdgeDetectionFilter.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class EdgeDetectionFilterInspector : public InspectorRenderer, RegisterInspectorRenderer<EdgeDetectionFilterInspector, Ext::EdgeDetectionFilter>
	{
		REGISTER_INSPECTOR(EdgeDetectionFilterInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
