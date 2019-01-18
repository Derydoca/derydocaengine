#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\SkinnedMeshRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class SkinnedMeshRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<SkinnedMeshRendererInspector, Components::SkinnedMeshRenderer>
	{
		REGISTER_INSPECTOR(SkinnedMeshRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
