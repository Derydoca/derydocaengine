#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ParticleContinuousFountain.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ParticleContinuousFountainInspector : public InspectorRenderer, RegisterInspectorRenderer<ParticleContinuousFountainInspector, Ext::ParticleContinuousFountain>
	{
		REGISTER_INSPECTOR(ParticleContinuousFountainInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
