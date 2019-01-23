#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\TessellatingQuad.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class TessellatingQuadInspector : public InspectorRenderer, RegisterInspectorRenderer<TessellatingQuadInspector, Ext::TessellatingQuad>
	{
		REGISTER_INSPECTOR(TessellatingQuadInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
