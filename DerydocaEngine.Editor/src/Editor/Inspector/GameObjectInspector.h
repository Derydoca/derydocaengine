#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "GameObject.h"
#include "Components\GameComponent.h"
#include "TypeNameLookup.h"

namespace DerydocaEngine::Editor::Inspector
{

	class GameObjectInspector : public InspectorRenderer
	{

		void renderComponent(std::shared_ptr<Components::GameComponent> component)
		{
			if (ImGui::CollapsingHeader(TypeNameLookup::getInstace().get(component).c_str()))
			{
				ImGui::TreePush();
				InspectorRendererFactory::getInstance().renderInspector(component);
				ImGui::TreePop();
			}
		}

		virtual void render(std::shared_ptr<Object> object)
		{
			auto gameObject = std::static_pointer_cast<GameObject>(object);
			
			std::string strObj = gameObject->getName();

			ImGui::Text("Game Object");
			
			Dgui::InputText("Name", gameObject->getName());

			InspectorRendererFactory::getInstance().renderInspector(gameObject->getTransform());

			for (auto component : gameObject->getComponents())
			{
				renderComponent(component);
			}
		}
	};

}
