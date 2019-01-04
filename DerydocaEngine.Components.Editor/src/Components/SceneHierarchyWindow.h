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
	};
}