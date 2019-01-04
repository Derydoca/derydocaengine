#pragma once
#include "Components\EditorWindowComponent.h"

namespace DerydocaEngine::Components
{
	class GameComponentInspectorWindow : public EditorWindowComponent, SelfRegister<GameComponentInspectorWindow>
	{
	public:
		GENINSTANCE(GameComponentInspectorWindow);

		GameComponentInspectorWindow();
		~GameComponentInspectorWindow();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Inspector"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }
	};

}