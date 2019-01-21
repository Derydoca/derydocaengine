#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\BezierCurveRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class BezierCurveRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<BezierCurveRendererInspector, Ext::BezierCurveRenderer>
	{
		REGISTER_INSPECTOR(BezierCurveRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
