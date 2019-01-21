#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "ComponentsExt\BloomFilter.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class BloomFilterInspector : public InspectorRenderer, RegisterInspectorRenderer<BloomFilterInspector, Ext::BloomFilter>
	{
		REGISTER_INSPECTOR(BloomFilterInspector);

		virtual void render(std::shared_ptr<Object> object);
	};

}
