#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\KeyboardMover.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class KeyboardMoverInspector : public InspectorRenderer, RegisterInspectorRenderer<KeyboardMoverInspector, Components::KeyboardMover>
	{
		REGISTER_INSPECTOR(KeyboardMoverInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
