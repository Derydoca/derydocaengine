#pragma once
#include "Components\EditorWindowComponent.h"
#include "Editor\SelectionManager.h"

namespace DerydocaEngine::Components
{
	class SceneHierarchyWindow : public EditorWindowComponent, SelfRegister<SceneHierarchyWindow>
	{
	public:
		GENINSTANCE(SceneHierarchyWindow);

		SceneHierarchyWindow();
		~SceneHierarchyWindow();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Scene Hierarchy"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);
	private:
		void renderGameObjectTreeNode(std::shared_ptr<GameObject> gameObject);

		std::weak_ptr<GameObject> m_SceneRoot;
		std::shared_ptr<Editor::SelectionGroup> m_SelectionGroup;
	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::SceneHierarchyWindow);
