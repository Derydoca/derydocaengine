#pragma once
#include "Editor\Inspector\ComponentInspectors\TransformInspector.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\WoodSliceTexture.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class WoodSliceTextureInspector : public InspectorRenderer, RegisterInspectorRenderer<WoodSliceTextureInspector, Ext::WoodSliceTexture>
	{
		REGISTER_INSPECTOR(WoodSliceTextureInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
