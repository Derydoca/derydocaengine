#include "EditorComponentsPch.h"
#include "EditorCameraWindow.h"
#include "Editor\EditorRenderer.h"

namespace DerydocaEngine::Components
{

	EditorCameraWindow::EditorCameraWindow() :
		m_windowWidth(0),
		m_windowHeight(0),
		m_renderTexture(std::make_shared<Rendering::RenderTexture>(200, 200)),
		m_camera(std::make_shared<Camera>())
	{
	}

	EditorCameraWindow::~EditorCameraWindow()
	{
	}

	void EditorCameraWindow::init()
	{
		getGameObject()->addComponent<Camera>(m_camera);
	}

	void EditorCameraWindow::render(const std::shared_ptr<Rendering::MatrixStack> matrixStack)
	{
		if (
			m_windowWidth != m_renderTexture->getWidth() ||
			m_windowHeight != m_renderTexture->getHeight() &&
			m_windowWidth > 0 &&
			m_windowHeight > 0
			)
		{
			m_renderTexture->initializeTexture(m_windowWidth, m_windowHeight);
		}

		Editor::EditorRenderer::GetInstance().renderEditorCamera(m_camera, m_renderTexture);
	}

	void EditorCameraWindow::renderWindow()
	{
		auto windowSize = ImGui::GetWindowSize();
		auto itemSize = ImGui::GetItemRectSize();
		m_windowWidth = windowSize.x;
		m_windowHeight = windowSize.y - itemSize.y * 2;
		ImGui::Image((ImTextureID)m_renderTexture->getRendererId(), { (float)m_windowWidth, (float)m_windowHeight }, { 0, 1 }, { 1, 0 });
	}

}
