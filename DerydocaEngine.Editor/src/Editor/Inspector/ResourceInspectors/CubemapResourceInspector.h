#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\CubemapResource.h"
#include "Rendering\Texture.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class CubemapResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::CubemapResource>(object);

			ImGui::Text("Cubemap");
			ImGui::Text(resource->getName().c_str());
		}
	};

}
