#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\TextRenderer.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class TextRendererInspector : public InspectorRenderer, RegisterInspectorRenderer<TextRendererInspector, Components::TextRenderer>
	{
		REGISTER_INSPECTOR(TextRendererInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
