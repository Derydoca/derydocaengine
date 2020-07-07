#pragma once
#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"

namespace DerydocaEngine::Components
{
	class InspectorWindow : public EditorWindowComponent, SelfRegister<InspectorWindow>
	{
	public:
		GENINSTANCE(InspectorWindow);

		InspectorWindow();
		~InspectorWindow();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Inspector"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);
	private:
		std::shared_ptr<Editor::SelectionGroup> m_SelectionGroup;

	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::InspectorWindow);
