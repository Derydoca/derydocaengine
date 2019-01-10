#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\SpriteSheetResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class SpriteSheetResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::SpriteSheetResource>(object);

			ImGui::Text("Sprite Sheet");
			ImGui::Text(resource->getName().c_str());
		}
	};

}
