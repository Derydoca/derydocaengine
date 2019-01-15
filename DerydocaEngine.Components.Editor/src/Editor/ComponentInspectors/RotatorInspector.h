#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\Rotator.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class RotatorInspector : public InspectorRenderer, RegisterInspectorRenderer<RotatorInspector, Components::Rotator>
	{
		REGISTER_INSPECTOR(RotatorInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
