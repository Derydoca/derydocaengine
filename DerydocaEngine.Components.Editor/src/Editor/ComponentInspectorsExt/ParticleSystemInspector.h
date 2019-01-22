#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ParticleSystem.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ParticleSystemInspector : public InspectorRenderer, RegisterInspectorRenderer<ParticleSystemInspector, Ext::ParticleSystem>
	{
		REGISTER_INSPECTOR(ParticleSystemInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
