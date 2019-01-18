#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\MaterialRefresher.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class MaterialRefresherInspector : public InspectorRenderer, RegisterInspectorRenderer<MaterialRefresherInspector, Components::MaterialRefresher>
	{
		REGISTER_INSPECTOR(MaterialRefresherInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
