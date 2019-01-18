#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\WasdMover.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class WasdMoverInspector : public InspectorRenderer, RegisterInspectorRenderer<WasdMoverInspector, Components::WasdMover>
	{
		REGISTER_INSPECTOR(WasdMoverInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
