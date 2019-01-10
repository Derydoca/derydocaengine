#pragma once
#include "EditorPch.h"
#include "InspectorRendererFactory.h"
#include "Resources\LevelResource.h"
#include "Editor\Inspector\InspectorRenderer.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Editor::Inspector
{

	class UndefinedInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			ImGui::Text("There is no inspector defined for this object type.");
			auto resource = std::static_pointer_cast<Resources::Resource>(object);
			if (resource)
			{
				ImGui::LabelText(resource->getSourceFilePath().c_str(), "Path");
			}
		}
	};

}
