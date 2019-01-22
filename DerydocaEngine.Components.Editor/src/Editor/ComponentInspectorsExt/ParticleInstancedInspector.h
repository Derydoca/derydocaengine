#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\ParticleInstanced.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class ParticleInstancedInspector : public InspectorRenderer, RegisterInspectorRenderer<ParticleInstancedInspector, Ext::ParticleInstanced>
	{
		REGISTER_INSPECTOR(ParticleInstancedInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
