#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class MeshRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<MeshRendererInspector, Components::MeshRenderer>
	{
		REGISTER_INSPECTOR(MeshRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
