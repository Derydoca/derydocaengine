#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\Terrain.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class TerrainInspector : public InspectorRenderer, RegisterInspectorRenderer<TerrainInspector, Components::Terrain>
	{
		REGISTER_INSPECTOR(TerrainInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
