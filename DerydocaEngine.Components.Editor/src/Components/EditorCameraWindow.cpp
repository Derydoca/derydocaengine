#include "EditorComponentsPch.h"
#include "EditorCameraWindow.h"
#include "Editor\EditorRenderer.h"

namespace DerydocaEngine::Components
{

	EditorCameraWindow::EditorCameraWindow() :
		m_renderTexture(std::make_shared<Rendering::RenderTexture>(200, 200))
	{
	}

	EditorCameraWindow::~EditorCameraWindow()
	{
	}

	void EditorCameraWindow::render(const std::shared_ptr<Rendering::MatrixStack> matrixStack)
	{
		Editor::EditorRenderer::GetInstance().renderEditorCamera(m_renderTexture);
	}

	void EditorCameraWindow::renderWindow()
	{
		ImGui::Image((ImTextureID)m_renderTexture->getRendererId(), { 200, 200 });
		ImGui::Text("Camera view goes here!");
	}

}
