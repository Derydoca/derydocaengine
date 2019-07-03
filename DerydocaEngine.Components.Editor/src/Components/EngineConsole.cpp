#include "EditorComponentsPch.h"
#include "EngineConsole.h"

namespace DerydocaEngine::Components
{

	EngineConsole::EngineConsole()
	{
		{
			Logging::LogMessage m;
			m.fileName = "file.cpp";
			m.functionName = "myFunc(int, float)";
			m.level = Logging::LogLevel::Info;
			m.line = 25;
			m.loggerName = "theLogger";
			m.message = "This is the message";
			m_messages.push_back(m);
		}
		{
			Logging::LogMessage m;
			m.fileName = "file.cpp";
			m.functionName = "myFunc(int, float)";
			m.level = Logging::LogLevel::Warn;
			m.line = 25;
			m.loggerName = "theLogger";
			m.message = "This is the warning";
			m_messages.push_back(m);
		}
		{
			Logging::LogMessage m;
			m.fileName = "file.cpp";
			m.functionName = "myFunc(int, float)";
			m.level = Logging::LogLevel::Err;
			m.line = 25;
			m.loggerName = "theLogger";
			m.message = "This is the error";
			m_messages.push_back(m);
		}
	}

	EngineConsole::~EngineConsole()
	{
	}

	void EngineConsole::renderWindow()
	{
		float detailsHeight = 100.0f;
		float childHeight = glm::max(ImGui::GetContentRegionAvail().y - detailsHeight, detailsHeight);

		static size_t selected = -1;
		ImGui::BeginChild("Logs", ImVec2(0.0f, childHeight), false);
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::Text("Severity"); ImGui::NextColumn();
		ImGui::Text("Message"); ImGui::NextColumn();
		ImGui::Separator();
		{
			for (size_t i = 0; i < m_messages.size(); i++)
			{
				Logging::LogMessage m = m_messages[i];
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_Text, logLevelToColor(m.level));
				
				if (ImGui::Selectable(m.functionName.c_str(), selected == i, ImGuiSelectableFlags_SpanAllColumns))
				{
					selected = i;
				}
				ImGui::NextColumn();
				ImGui::Text(m.message.c_str());
				ImGui::NextColumn();

				ImGui::PopStyleColor();
				ImGui::PopID();
			}
		}
		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::BeginChild("Details", ImVec2(0.0f, detailsHeight - 10.0f), false);
		if (selected >= 0 && selected < m_messages.size())
		{
			Logging::LogMessage m = m_messages[selected];
			ImGui::Text(m.message.c_str());
			ImGui::Text(m.functionName.c_str());
			ImGui::Text("%s:%d", m.fileName.c_str(), m.line);
		}
		else
		{
			ImGui::Text("Select a log to see the details.");
		}
		ImGui::EndChild();
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