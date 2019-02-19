#include "EditorComponentsPch.h"
#include "EditorCameraWindow.h"
#include "Editor\Inspector\InspectorRendererFactory.h"

namespace DerydocaEngine::Components
{
	const float PROPERTIES_PANEL_WIDTH = 400.0f;
	const float VERTICAL_PADDING = 70.0f;

	EditorCameraWindow::EditorCameraWindow() :
		SceneViewerWindow(),
		m_showPropertiesPanel(false)
	{
	}

	EditorCameraWindow::~EditorCameraWindow()
	{
	}

	void EditorCameraWindow::renderWindow()
	{
		SceneViewerWindow::updateDisplayProperties();

		// Show button to show/hide the properties panel
		if (ImGui::Button(m_showPropertiesPanel ? "Hide Properties" : "Show Properties"))
		{
			m_showPropertiesPanel = !m_showPropertiesPanel;
		}
		// Also add status text to let the user know if the window is focued
		ImGui::SameLine();
		ImGui::Text(isActive() ? "Focused" : "Not Focused");

		// Render the scene view
		SceneViewerWindow::renderViewToWindow();

		// Render the properties panel if it is marked to be visible
		if (m_showPropertiesPanel)
		{
			ImGui::SameLine();
			if (ImGui::BeginChild(boost::uuids::to_string(getId()).c_str(), { 400.0f, (float)getDisplayHeight() }))
			{
				Editor::Inspector::InspectorRendererFactory::getInstance().renderInspector(getCamera());
			}
			ImGui::EndChild();
		}
	}

	glm::vec2 EditorCameraWindow::getViewPadding()
	{
		return glm::vec2(
			m_showPropertiesPanel ? PROPERTIES_PANEL_WIDTH : 0,
			VERTICAL_PADDING
		);
	}

}
