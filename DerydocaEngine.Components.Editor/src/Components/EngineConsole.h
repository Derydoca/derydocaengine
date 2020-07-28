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

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::EditorWindowComponent);
	private:
		ImVec4 logLevelToColor(Logging::LogLevel level);
		void renderMenuBar();
	private:
		size_t m_Selected;
		Logging::LogLevel m_VisibleTypeFlags;
		Logging::LogDomain m_VisibleDomainFlags;
		std::vector<Logging::LogMessage>* m_Messages;
	};
}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::EngineConsole, 0);
