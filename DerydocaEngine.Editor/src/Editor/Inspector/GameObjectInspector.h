#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "GameObject.h"

namespace DerydocaEngine::Editor::Inspector
{

	class GameObjectInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto gameObject = std::static_pointer_cast<GameObject>(object);
			
			std::string strObj = gameObject->getName();

			ImGui::Text("Game Object");
			
			Dgui::InputText("Name", gameObject->getName());

			InspectorRendererFactory::getInstance().renderInspector(gameObject->getTransform());
		}
	};

}
