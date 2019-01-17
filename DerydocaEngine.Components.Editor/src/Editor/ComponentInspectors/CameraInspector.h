#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class CameraInspector : public InspectorRenderer, RegisterInspectorRenderer<CameraInspector, Components::Camera> 
	{
		REGISTER_INSPECTOR(CameraInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
