#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\RasterFontResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class RasterFontResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::RasterFontResource>(object);

			ImGui::Text("Raster Font");
			ImGui::Text(resource->getName().c_str());
		}
	};

}
