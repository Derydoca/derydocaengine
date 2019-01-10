#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\LevelResource.h"
#include "Editor\Inspector\InspectorRenderer.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class LevelResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto levelResource = std::static_pointer_cast<Resources::LevelResource>(object);

			ImGui::Text(levelResource->getName().c_str());
			if (ImGui::Button("Load"))
			{
				Scenes::SceneManager::getInstance().loadScene(levelResource);
			}
		}
	};

}
