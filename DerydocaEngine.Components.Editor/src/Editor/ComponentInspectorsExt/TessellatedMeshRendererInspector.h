#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\TessellatedMeshRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class TessellatedMeshRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<TessellatedMeshRendererInspector, Ext::TessellatedMeshRenderer>
	{
		REGISTER_INSPECTOR(TessellatedMeshRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
