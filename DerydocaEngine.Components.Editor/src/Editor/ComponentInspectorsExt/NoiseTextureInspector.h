#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\NoiseTexture.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class NoiseTextureInspector : public InspectorRenderer, RegisterInspectorRenderer<NoiseTextureInspector, Ext::NoiseTexture>
	{
		REGISTER_INSPECTOR(NoiseTextureInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
