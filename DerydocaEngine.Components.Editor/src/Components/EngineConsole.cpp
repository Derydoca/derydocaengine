#include "EditorComponentsPch.h"
#include "EngineConsole.h"

namespace DerydocaEngine::Components
{

	EngineConsole::EngineConsole() :
		m_selected(-1),
		m_visibleTypeFlags(static_cast<Logging::LogLevel>(Logging::LogLevel::All & ~(Logging::LogLevel::Trace))),
		m_messages()
	{
		m_messages = &Logging::Log::GetCoreSync()->getMessages();
	}

	EngineConsole::~EngineConsole()
	{
	}

	void selectableLogLevel(const char* text, const Logging::LogLevel level, Logging::LogLevel& visibleTypeFlags)
	{
		if (ImGui::Selectable(text, visibleTypeFlags & level, 0, ImVec2(50.0f, 0.0f)))
		{
			visibleTypeFlags = (Logging::LogLevel)(visibleTypeFlags ^ level);
		}
	}

	void EngineConsole::renderWindow()
	{
		renderMenuBar();

		float detailsHeight = 100.0f;
		float childHeight = glm::max(ImGui::GetContentRegionAvail().y - detailsHeight, detailsHeight);

		ImGui::BeginChild("Logs", ImVec2(0.0f, childHeight), false);
		{
			for (size_t i = 0; i < m_messages->size(); i++)
			{
				Logging::LogMessage m = m_messages->at(i);

				if ((m_visibleTypeFlags & m.level) == 0)
				{
					continue;
				}

				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_Text, logLevelToColor(m.level));
				
				if (ImGui::Selectable(m.message.c_str(), m_selected == i, ImGuiSelectableFlags_SpanAllColumns))
				{
					m_selected = i;
				}

				ImGui::PopStyleColor();
				ImGui::PopID();
			}
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::BeginChild("Details", ImVec2(0.0f, detailsHeight - 10.0f), false);
		if (m_selected >= 0 && m_selected < m_messages->size() && (m_visibleTypeFlags & m_messages->at(m_selected).level))
		{
			Logging::LogMessage m = m_messages->at(m_selected);
			if (m.line > 0)
			{
				ImGui::TextWrapped("%s(%d)%s", m.fileName.c_str(), m.line, m.functionName.c_str());
			}
			ImGui::TextWrapped(m.message.c_str());
		}
		else
		{
			ImGui::Text("Select a log to see the details.");
		}
		ImGui::EndChild();
	}

	void EngineConsole::renderMenuBar()
	{
		ImGui::BeginMenuBar();
		if (ImGui::Button("Clear"))
		{
			m_messages->clear();
			m_selected = -1;
		}

		ImGui::Spacing();

		selectableLogLevel("Trace", Logging::LogLevel::Trace, m_visibleTypeFlags);
		selectableLogLevel("Info", Logging::LogLevel::Info, m_visibleTypeFlags);
		selectableLogLevel("Warn", Logging::LogLevel::Warn, m_visibleTypeFlags);
		selectableLogLevel("Error", Logging::LogLevel::Err, m_visibleTypeFlags);
		ImGui::EndMenuBar();
	}

	ImVec4 EngineConsole::logLevelToColor(Logging::LogLevel level)
	{
		switch (level)
		{
		case Logging::LogLevel::Critical:
		case Logging::LogLevel::Err:
			return { 1.0, 0.0, 0.0, 1.0 };
		case Logging::LogLevel::Warn:
			return { 1.0, 1.0, 0.0, 1.0 };
		default:
			return { 1.0, 1.0, 1.0, 1.0 };
		}
	}

}