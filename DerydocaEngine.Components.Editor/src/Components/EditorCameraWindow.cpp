#include "EditorComponentsPch.h"
#include "EditorCameraWindow.h"
#include "Editor\EditorRenderer.h"

namespace DerydocaEngine::Components
{

	EditorCameraWindow::EditorCameraWindow() :
		m_windowWidth(0),
		m_windowHeight(0),
		m_mouse(0),
		m_mouseSensitivityX(0.005f),
		m_mouseSensitivityY(0.005f),
		m_minXRot(-glm::half_pi<float>()),
		m_maxXRot(glm::half_pi<float>()),
		m_eulerRot(),
		m_keyboard(0),
		m_moveSpeed(5.0f),
		m_renderTexture(std::make_shared<Rendering::RenderTexture>(200, 200)),
		m_camera(std::make_shared<Camera>()),
		m_cameraTransform()
	{
		m_mouse = Input::InputManager::getInstance().getMouse();
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	EditorCameraWindow::~EditorCameraWindow()
	{
	}

	void EditorCameraWindow::init()
	{
		getGameObject()->addComponent<Camera>(m_camera);
		m_cameraTransform = m_camera->getGameObject()->getTransform();
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

	void EditorCameraWindow::update(const float deltaTime)
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

	void EditorCameraWindow::renderWindow()
	{
		m_isActive = ImGui::IsWindowFocused();

		auto windowSize = ImGui::GetWindowSize();
		auto itemSize = ImGui::GetItemRectSize();
		m_windowWidth = windowSize.x;
		m_windowHeight = windowSize.y - itemSize.y * 3;
		ImGui::Text(m_isActive ? "Focused" : "Not Focused");
		ImGui::Image((ImTextureID)m_renderTexture->getRendererId(), { (float)m_windowWidth, (float)m_windowHeight }, { 0, 1 }, { 1, 0 });
	}

}
