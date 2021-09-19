#include "EditorComponentsPch.h"
#include "EditorCameraWindow.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Editor\EditorRenderer.h"
#include "SceneCameraInput\WasdControlStrategy.h"

namespace DerydocaEngine::Components
{
	const float PROPERTIES_PANEL_WIDTH = 400.0f;
	const float VERTICAL_PADDING = 70.0f;

	EditorCameraWindow::EditorCameraWindow() :
		SceneViewerWindow(std::make_shared<SceneCameraInput::WasdControlStrategy>()),
		m_ShowPropertiesPanel(false)
	{
	}

	EditorCameraWindow::~EditorCameraWindow()
	{
	}

	void EditorCameraWindow::renderWindow()
	{
		SceneViewerWindow::updateDisplayProperties();

		//// Show button to show/hide the properties panel
		//if (ImGui::Button(m_ShowPropertiesPanel ? "Hide Properties" : "Show Properties"))
		//{
		//	m_ShowPropertiesPanel = !m_ShowPropertiesPanel;
		//}
		//// Also add status text to let the user know if the window is focued
		//ImGui::SameLine();
		//ImGui::Text(isActive() ? "Focused" : "Not Focused");

		// Render the scene view
		SceneViewerWindow::renderViewToWindow();

		// Render the properties panel if it is marked to be visible
		if (m_ShowPropertiesPanel)
		{
			ImGui::SameLine();
			if (ImGui::BeginChild(getId().to_string().c_str(), { PROPERTIES_PANEL_WIDTH, (float)getDisplayHeight() }))
			{
				Editor::Inspector::InspectorRendererFactory::getInstance().renderInspector(getCamera());
			}
			ImGui::EndChild();
		}
	}

	glm::vec2 EditorCameraWindow::getViewPadding()
	{
		return glm::vec2(
			m_ShowPropertiesPanel ? PROPERTIES_PANEL_WIDTH : 0,
			VERTICAL_PADDING
		);
	}

	void EditorCameraWindow::renderToActiveBuffer()
	{
		Editor::EditorRenderer::GetInstance().renderEditorCameraToActiveBuffer(getCamera(), static_cast<int>(getDisplayWidth()), static_cast<int>(getDisplayHeight()));
	}

}
