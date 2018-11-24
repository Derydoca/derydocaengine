#include "Components\WasdMover.h"

#include "Input\InputManager.h"
#include "Input\Keyboard.h"
#include "Input\Mouse.h"
#include "sdl2\SDL.h"

namespace DerydocaEngine::Components
{

	WasdMover::WasdMover()
	{
		m_mouse = Input::InputManager::getInstance().getMouse();
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	WasdMover::~WasdMover()
	{
	}

	void WasdMover::init()
	{
		m_transform = getGameObject()->getTransform();
	}

	void WasdMover::update(float const& deltaTime) {

		// Set the mouse to relative mode when the right mouse button is down
		if (m_mouse->isKeyDownFrame(2)) {
			m_mouse->setRelative(true);
		}
		else if (m_mouse->isKeyUpFrame(2)) {
			m_mouse->setRelative(false);
		}

		// Only allow the camera to be moved when the right mouse button is down
		if (m_mouse->isKeyDown(2)) {
			// Reset the momentum each frame
			m_localMomentum = glm::vec3();

			float moveSpeed = m_moveSpeed * deltaTime;
			if (m_keyboard->isKeyDown(SDLK_z)) {
				moveSpeed *= m_fastMoveMultiplier;
			}

			// Forward/backward
			if (m_keyboard->isKeyDown(SDLK_w)) {
				m_localMomentum.z -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_s)) {
				m_localMomentum.z += moveSpeed;
			}

			// Left/right
			if (m_keyboard->isKeyDown(SDLK_a)) {
				m_localMomentum.x -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_d)) {
				m_localMomentum.x += moveSpeed;
			}

			// Up/down
			if (m_keyboard->isKeyDown(SDLK_q)) {
				m_localMomentum.y -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_e)) {
				m_localMomentum.y += moveSpeed;
			}
			// Convert the local direction to global direction
			glm::vec4 globalMomentum = m_transform->getModel() * glm::vec4(m_localMomentum, 0);

			// Translate
			m_transform->setPos(m_transform->getPos() + glm::vec3(globalMomentum));

			// Update the rotations based on mouse movement
			glm::ivec2 diff = m_mouse->getRelativeMovement();
			m_eulerRot.x -= (float)diff.y * m_mouseSensitivityX;
			m_eulerRot.y -= (float)diff.x * m_mouseSensitivityY;

			// Clamp the vertical look
			if (m_eulerRot.x < m_minXRot)
			{
				m_eulerRot.x = m_minXRot;
			}
			else if (m_eulerRot.x > m_maxXRot)
			{
				m_eulerRot.x = m_maxXRot;
			}

			if (m_keyboard->isKeyDownFrame(SDLK_LEFTBRACKET))
			{
				m_eulerRot.y -= 90.0f * 0.0174533f;
			}
			if (m_keyboard->isKeyDownFrame(SDLK_RIGHTBRACKET))
			{
				m_eulerRot.y += 90.0f * 0.0174533f;
			}

			// Rotate
			glm::fquat newQuat =
				glm::rotate(m_eulerRot.y, glm::vec3(0, 1, 0))
				*
				glm::rotate(m_eulerRot.x, glm::vec3(1, 0, 0))
				;
			m_transform->setQuat(newQuat);
		}
	}

	void WasdMover::deserialize(YAML::Node const& node)
	{
		m_moveSpeed = node["moveSpeed"].as<float>();
		m_mouseSensitivityX = node["mouseSensitivityX"].as<float>();
		m_mouseSensitivityY = node["mouseSensitivityY"].as<float>();
		m_fastMoveMultiplier = node["fastMoveMultiplier"].as<float>();
		m_slowMoveMultiplier = node["slowMoveMultiplier"].as<float>();
		m_minXRot = node["minXRot"].as<float>();
		m_maxXRot = node["maxXRot"].as<float>();
	}

}