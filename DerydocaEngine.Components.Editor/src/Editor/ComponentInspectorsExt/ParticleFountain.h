#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ParticleFountain.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ParticleFountainInspector : public InspectorRenderer, RegisterInspectorRenderer<ParticleFountainInspector, Ext::ParticleFountain>
	{
		REGISTER_INSPECTOR(ParticleFountainInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
