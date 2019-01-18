#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\SpriteRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class SpriteRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<SpriteRendererInspector, Components::SpriteRenderer>
	{
		REGISTER_INSPECTOR(SpriteRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
