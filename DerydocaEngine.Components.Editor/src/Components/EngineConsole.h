#pragma once
#include "Components\EditorWindowComponent.h"

namespace DerydocaEngine::Components
{
	class EngineConsole : public EditorWindowComponent, SelfRegister<EngineConsole>
	{
	public:
		GENINSTANCE(EngineConsole);

		EngineConsole();
		~EngineConsole();

		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Console"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_MenuBar; }
	private:
	};
}