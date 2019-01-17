#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\FrameStats.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class FrameStatsInspector : public InspectorRenderer, RegisterInspectorRenderer<FrameStatsInspector, Components::FrameStats>
	{
		REGISTER_INSPECTOR(FrameStatsInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
