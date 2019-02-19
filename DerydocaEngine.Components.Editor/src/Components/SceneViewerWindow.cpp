#include "EditorComponentsPch.h"
#include "SceneViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Components
{

	SceneViewerWindow::SceneViewerWindow() :
		m_displayWidth(0),
		m_displayHeight(0),
		m_mouse(0),
		m_mouseSensitivityX(0.005f),
		m_mouseSensitivityY(0.005f),
		m_minXRot(-glm::half_pi<float>()),
		m_maxXRot(glm::half_pi<float>()),
		m_eulerRot(),
		m_keyboard(0),
		m_moveSpeed(5.0f),
		m_renderTexture(std::make_shared<Rendering::RenderTexture>()),
		m_camera(std::make_shared<Camera>(Camera::NoRegister({}))),
		m_cameraTransform()
	{
		m_renderTexture->initializeTexture(200, 200);
		m_mouse = Input::InputManager::getInstance().getMouse();
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
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

		if (m_mouse->isKeyDownFrame(2))
		{
			m_mouse->setRelative(true);
		}
		else if (m_mouse->isKeyUpFrame(2))
		{
			m_mouse->setRelative(false);
		}

		if (m_mouse->isKeyDown(2))
		{
			{ // Keyboard movement

			// Reset the momentum each frame
				auto localMomentum = glm::vec3();

				float moveSpeed = m_moveSpeed * deltaTime;

				// Forward/backward
				if (m_keyboard->isKeyDown(SDLK_w)) {
					localMomentum.z -= moveSpeed;
				}
				if (m_keyboard->isKeyDown(SDLK_s)) {
					localMomentum.z += moveSpeed;
				}

				// Left/right
				if (m_keyboard->isKeyDown(SDLK_a)) {
					localMomentum.x -= moveSpeed;
				}
				if (m_keyboard->isKeyDown(SDLK_d)) {
					localMomentum.x += moveSpeed;
				}

				// Up/down
				if (m_keyboard->isKeyDown(SDLK_q)) {
					localMomentum.y -= moveSpeed;
				}
				if (m_keyboard->isKeyDown(SDLK_e)) {
					localMomentum.y += moveSpeed;
				}
				// Convert the local direction to global direction
				glm::vec4 globalMomentum = m_cameraTransform->getModel() * glm::vec4(localMomentum, 0);

				// Translate
				m_cameraTransform->setPos(m_cameraTransform->getPos() + glm::vec3(globalMomentum));

			} // End keyboard movement

			{ // Mouse look
				glm::ivec2 diff = m_mouse->getRelativeMovement();
				m_eulerRot.x -= (float)diff.y * m_mouseSensitivityX;
				m_eulerRot.y -= (float)diff.x * m_mouseSensitivityY;

				if (m_eulerRot.x < m_minXRot)
				{
					m_eulerRot.x = m_minXRot;
				}
				else if (m_eulerRot.x > m_maxXRot)
				{
					m_eulerRot.x = m_maxXRot;
				}

				glm::fquat newQuat =
					glm::rotate(m_eulerRot.y, glm::vec3(0, 1, 0))
					*
					glm::rotate(m_eulerRot.x, glm::vec3(1, 0, 0))
					;
				m_cameraTransform->setQuat(newQuat);
			} // End mouse look
		}
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