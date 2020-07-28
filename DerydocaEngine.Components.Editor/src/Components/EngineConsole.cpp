#include "EditorComponentsPch.h"
#include "EngineConsole.h"

namespace DerydocaEngine::Components
{

	EngineConsole::EngineConsole() :
		m_Selected(-1),
		m_VisibleTypeFlags(static_cast<Logging::LogLevel>(Logging::LogLevel::All & ~(Logging::LogLevel::Trace))),
		m_VisibleDomainFlags(static_cast<Logging::LogDomain>(Logging::LogDomain::Engine | Logging::LogDomain::Client)),
		m_Messages()
	{
		m_Messages = &Logging::Log::GetConsoleSink()->getMessages();
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
			if (m_VisibleTypeFlags == 0)
			{
				ImGui::Text("No log types are marked to be visible.");
				if (ImGui::Button("Fix it!"))
				{
					m_VisibleTypeFlags = Logging::LogLevel::All;
				}
			}
			else if (m_VisibleDomainFlags == 0)
			{
				ImGui::Text("No log domains are marked to be visible.");
				if (ImGui::Button("Fix it!"))
				{
					m_VisibleDomainFlags = static_cast<Logging::LogDomain>(Logging::LogDomain::Client | Logging::LogDomain::Engine);
				}
			}
			else
			{
				for (size_t i = 0; i < m_Messages->size(); i++)
				{
					Logging::LogMessage m = m_Messages->at(i);

					// Skip rendering this log if it isn't of a visible type or domain
					if ((m_VisibleTypeFlags & m.level) == 0 || (m_VisibleDomainFlags & m.domain) == 0)
					{
						continue;
					}

					ImGui::PushID(static_cast<int>(i));
					ImGui::PushStyleColor(ImGuiCol_Text, logLevelToColor(m.level));

					if (ImGui::Selectable(m.message.c_str(), m_Selected == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						m_Selected = i;
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
			m_Selected >= 0 && m_Selected < m_Messages->size()
			&& (m_VisibleTypeFlags & m_Messages->at(m_Selected).level)
			&& (m_VisibleDomainFlags & m_Messages->at(m_Selected).domain)
			)
		{
			Logging::LogMessage m = m_Messages->at(m_Selected);
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
			m_Messages->clear();
			m_Selected = -1;
		}

		ImGui::Spacing();

		selectableLogLevel("Info", Logging::LogLevel::Info, m_VisibleTypeFlags, true);
		selectableLogLevel("Warn", Logging::LogLevel::Warn, m_VisibleTypeFlags, true);
		selectableLogLevel("Error", Logging::LogLevel::Err, m_VisibleTypeFlags, true);

		if (ImGui::Button("..."))
		{
			ImGui::OpenPopup("log_filters");
		}

		if (ImGui::BeginPopup("log_filters"))
		{
			selectableLogLevel("Trace", Logging::LogLevel::Trace, m_VisibleTypeFlags, false);
			selectableLogLevel("Debug", Logging::LogLevel::Debug, m_VisibleTypeFlags, false);
			selectableLogLevel("Critical", Logging::LogLevel::Critical, m_VisibleTypeFlags, false);

			ImGui::Spacing();

			selectableLogDomain("Engine", Logging::LogDomain::Engine, m_VisibleDomainFlags);
			selectableLogDomain("Client", Logging::LogDomain::Client, m_VisibleDomainFlags);

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