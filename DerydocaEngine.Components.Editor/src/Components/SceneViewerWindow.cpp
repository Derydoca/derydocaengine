#include "EditorComponentsPch.h"
#include "SceneViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Components
{

	SceneViewerWindow::SceneViewerWindow(std::shared_ptr<SceneCameraInput::SceneCameraInputStrategy> inputStrategy) :
		m_sceneCameraInputStrategy(inputStrategy),
		m_isActive(false),
		m_isViewportHovered(false),
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
		auto skyboxMaterialResource = Editor::EditorRenderer::GetInstance().getEditorSkyboxMaterialResource();
		if (skyboxMaterialResource != nullptr)
		{
			m_camera->setSkybox(skyboxMaterialResource);
			m_camera->setClearMode(Components::Camera::ClearMode::SkyboxClear);
		}

		// Store the camera's transform for later use
		m_cameraTransform = m_camera->getGameObject()->getTransform();
		m_cameraTransform->setPosition({ 0.0f, 0.5f, 1.0f });

		// Set the initial camera position
		m_sceneCameraInputStrategy->updateCameraTransform(m_cameraTransform);
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
			m_renderTexture->initializeTexture(static_cast<int>(m_displayWidth), static_cast<int>(m_displayHeight));
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

		m_sceneCameraInputStrategy->updateActiveState(m_isViewportHovered);

		if (m_sceneCameraInputStrategy->updateInput(deltaTime, m_isViewportHovered))
		{
			m_sceneCameraInputStrategy->updateCameraTransform(m_cameraTransform);
		}
	}

	void SceneViewerWindow::updateDisplayProperties()
	{
		// Store the window's active state
		m_isActive = ImGui::IsWindowFocused();

		// Define the display size of the render area
		auto contentArea = ImGui::GetContentRegionAvail();
		m_displayWidth = contentArea.x;
		m_displayHeight = contentArea.y;

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
#pragma warning(suppress: 4312)
		ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_renderTexture->getRendererId()), { m_displayWidth, m_displayHeight }, { 0, 1 }, { 1, 0 }, 0);
		m_isViewportHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);
	}

}