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

			ImGui::DragFloat3("Position", &transform->getPosition()[0], 0.005f);
			Dgui::InputQuaternion("Rotation", transform->getQuaternion());
			ImGui::DragFloat3("Scale", &transform->getScale()[0], 0.005f);
		}
	};

}
