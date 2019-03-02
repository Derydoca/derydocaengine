#include "EditorComponentsPch.h"
#include "SceneViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Components
{

	SceneViewerWindow::SceneViewerWindow(std::shared_ptr<SceneCameraInput::SceneCameraInputStrategy> inputStrategy) :
		m_sceneCameraInputStrategy(inputStrategy),
		m_displayWidth(0),
		m_displayHeight(0),
		m_renderTexture(std::make_shared<Rendering::RenderTexture>()),
		m_camera(std::make_shared<Camera>(Camera::NoRegister({}))),
		m_cameraTransform()
	{
		m_renderTexture->initializeTexture(200, 200);
	}

	SceneViewerWindow::~SceneViewerWindow()
	{
	}

	void SceneViewerWindow::init()
	{
		// Add the camera component under this game object
		getGameObject()->addComponent<Camera>(m_camera);
		m_camera->init();
		m_camera->postInit();

		// Set the skybox to the editor default
		auto skyboxMaterial = Editor::EditorRenderer::GetInstance().getEditorSkyboxMaterial();
		if (skyboxMaterial != nullptr)
		{
			m_camera->setSkybox(skyboxMaterial);
			m_camera->setClearMode(Components::Camera::ClearMode::SkyboxClear);
		}

		// Store the camera's transform for later use
		m_cameraTransform = m_camera->getGameObject()->getTransform();
	}

	void SceneViewerWindow::render(const std::shared_ptr<Rendering::MatrixStack> matrixStack)
	{
		// Update the render texture size if the display area has changed since the last frame
		if (
			m_displayWidth != m_renderTexture->getWidth() ||
			m_displayHeight != m_renderTexture->getHeight() &&
			m_displayWidth > 0 &&
			m_displayHeight > 0
			)
		{
			m_renderTexture->initializeTexture(m_displayWidth, m_displayHeight);
		}

		// Clear the render texture and then render to it
		m_renderTexture->bindAsRenderTexture();
		renderToActiveBuffer();
		Rendering::GraphicsAPI::bindFramebuffer(0);
	}

	void SceneViewerWindow::update(const float deltaTime)
	{
		if (!m_isActive)
		{
			return;
		}

		m_sceneCameraInputStrategy->update(deltaTime, m_camera->getGameObject()->getTransform());
	}

	void SceneViewerWindow::updateDisplayProperties()
	{
		// Store the window's active state
		m_isActive = ImGui::IsWindowFocused();

		// Get useful information about the window
		auto windowSize = ImGui::GetWindowSize();
		auto itemSize = ImGui::GetItemRectSize();
		auto windowPos = ImGui::GetWindowPos();
		auto frameHeight = ImGui::GetFrameHeight();
		auto viewport = ImGui::GetWindowViewport();

		// Define the display size of the render area
		glm::vec2 padding = getViewPadding();
		m_displayWidth = windowSize.x - padding.x;
		m_displayHeight = windowSize.y - padding.y;

		// Default the display size to 1x1 if the values are invalid
		if (m_displayWidth <= 0 || m_displayHeight <= 0)
		{
			m_displayWidth = 1;
			m_displayHeight = 1;
		}
	}

	void SceneViewerWindow::renderViewToWindow()
	{
		// Render the camera's view to the window
		ImGui::Image((ImTextureID)m_renderTexture->getRendererId(), { m_displayWidth, m_displayHeight }, { 0, 1 }, { 1, 0 });
	}

}