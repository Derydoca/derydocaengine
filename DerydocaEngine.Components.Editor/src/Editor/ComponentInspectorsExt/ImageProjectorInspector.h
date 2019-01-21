#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ImageProjector.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ImageProjectorInspector : public InspectorRenderer, RegisterInspectorRenderer<ImageProjectorInspector, Ext::ImageProjector>
	{
		REGISTER_INSPECTOR(ImageProjectorInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
