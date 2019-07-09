#include "EditorComponentsPch.h"
#include "EngineConsole.h"

namespace DerydocaEngine::Components
{

	EngineConsole::EngineConsole() :
		m_selected(-1),
		m_visibleTypeFlags(static_cast<Logging::LogLevel>(Logging::LogLevel::All & ~(Logging::LogLevel::Trace))),
		m_visibleDomainFlags(static_cast<Logging::LogDomain>(Logging::LogDomain::Engine | Logging::LogDomain::Client)),
		m_messages()
	{
		m_messages = &Logging::Log::GetConsoleSink()->getMessages();
	}

	EngineConsole::~EngineConsole()
	{
	}

	void selectableLogLevel(const char* text, const Logging::LogLevel level, Logging::LogLevel& visibleTypeFlags, bool small)
	{
		if (
			(small && ImGui::Selectable(text, visibleTypeFlags & level, 0, ImVec2(50.0f, 0.0f)))
			||
			(!small && ImGui::MenuItem(text, 0, visibleTypeFlags & level, true))
			)
		{
			visibleTypeFlags = (Logging::LogLevel)(visibleTypeFlags ^ level);
		}
	}

	void selectableLogDomain(const char* text, const Logging::LogDomain domain, Logging::LogDomain& visibleDomains)
	{
		if (ImGui::MenuItem(text, 0, visibleDomains & domain, true))
		{
			visibleDomains = (Logging::LogDomain)(visibleDomains ^ domain);
		}
	}

	void EngineConsole::renderWindow()
	{
		renderMenuBar();

		float detailsHeight = 100.0f;
		float childHeight = glm::max(ImGui::GetContentRegionAvail().y - detailsHeight, detailsHeight);

		ImGui::BeginChild("Logs", ImVec2(0.0f, childHeight), false);
		{
			if (m_visibleTypeFlags == 0)
			{
				ImGui::Text("No log types are marked to be visible.");
				if (ImGui::Button("Fix it!"))
				{
					m_visibleTypeFlags = Logging::LogLevel::All;
				}
			}
			else if (m_visibleDomainFlags == 0)
			{
				ImGui::Text("No log domains are marked to be visible.");
				if (ImGui::Button("Fix it!"))
				{
					m_visibleDomainFlags = static_cast<Logging::LogDomain>(Logging::LogDomain::Client | Logging::LogDomain::Engine);
				}
			}
			else
			{
				for (size_t i = 0; i < m_messages->size(); i++)
				{
					Logging::LogMessage m = m_messages->at(i);

					// Skip rendering this log if it isn't of a visible type or domain
					if ((m_visibleTypeFlags & m.level) == 0 || (m_visibleDomainFlags & m.domain) == 0)
					{
						continue;
					}

					ImGui::PushID(static_cast<int>(i));
					ImGui::PushStyleColor(ImGuiCol_Text, logLevelToColor(m.level));

					if (ImGui::Selectable(m.message.c_str(), m_selected == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						m_selected = i;
					}

					ImGui::PopStyleColor();
					ImGui::PopID();
				}
			}
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::BeginChild("Details", ImVec2(0.0f, detailsHeight - 10.0f), false);
		if (
			m_selected >= 0 && m_selected < m_messages->size()
			&& (m_visibleTypeFlags & m_messages->at(m_selected).level)
			&& (m_visibleDomainFlags & m_messages->at(m_selected).domain)
			)
		{
			Logging::LogMessage m = m_messages->at(m_selected);
			ImGui::TextWrapped(m.message.c_str());

			ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			ImGui::PushStyleColor(ImGuiCol_Text, { textColor.x, textColor.y, textColor.z, 0.5f });
			if (m.line > 0)
			{
				ImGui::TextWrapped("%s:%d @ %s", m.fileName.c_str(), m.line, m.functionName.c_str());
			}
			std::time_t time_t = std::chrono::system_clock::to_time_t(m.time);
			ImGui::Text(std::ctime(&time_t));
			ImGui::PopStyleColor();

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

		selectableLogLevel("Info", Logging::LogLevel::Info, m_visibleTypeFlags, true);
		selectableLogLevel("Warn", Logging::LogLevel::Warn, m_visibleTypeFlags, true);
		selectableLogLevel("Error", Logging::LogLevel::Err, m_visibleTypeFlags, true);

		if (ImGui::Button("..."))
		{
			ImGui::OpenPopup("log_filters");
		}

		if (ImGui::BeginPopup("log_filters"))
		{
			selectableLogLevel("Trace", Logging::LogLevel::Trace, m_visibleTypeFlags, false);
			selectableLogLevel("Debug", Logging::LogLevel::Debug, m_visibleTypeFlags, false);
			selectableLogLevel("Critical", Logging::LogLevel::Critical, m_visibleTypeFlags, false);

			ImGui::Spacing();

			selectableLogDomain("Engine", Logging::LogDomain::Engine, m_visibleDomainFlags);
			selectableLogDomain("Client", Logging::LogDomain::Client, m_visibleDomainFlags);

			ImGui::EndPopup();
		}

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