#pragma once
#include "Components\EditorWindowComponent.h"

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

	private:
		void renderGameObjectTreeNode(std::shared_ptr<GameObject> gameObject);

		std::weak_ptr<GameObject> m_sceneRoot;
	};
}