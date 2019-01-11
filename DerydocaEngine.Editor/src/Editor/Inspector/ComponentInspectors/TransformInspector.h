#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Components\Transform.h"
#include "Dgui\InputQuaternion.h"

namespace DerydocaEngine::Editor::Inspector::ComponentInspectors
{

	class TransformInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto transform = std::static_pointer_cast<Components::Transform>(object);

			ImGui::InputFloat3("Position", &transform->getPos()[0]);
			//ImGui::InputFloat3("Rotation", &transform->setEulerAngles()[0]);
			Dgui::InputQuaternion("Rotation", transform->getQuat());
			ImGui::InputFloat3("Scale", &transform->getScale()[0]);
			
		}
	};

}
