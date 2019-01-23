#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\WaveDisplacement.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class WaveDisplacementInspector : public InspectorRenderer, RegisterInspectorRenderer<WaveDisplacementInspector, Ext::WaveDisplacement>
	{
		REGISTER_INSPECTOR(WaveDisplacementInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
