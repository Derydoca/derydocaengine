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
		ImVec4 logLevelToColor(Logging::LogLevel level);
		void renderMenuBar();
	private:
		size_t m_selected;
		Logging::LogLevel m_visibleTypeFlags;
		std::vector<Logging::LogMessage> m_messages;
	};
}