#include "WasdMover.h"

WasdMover::~WasdMover()
{
}

void WasdMover::init()
{
	m_transform = getGameObject()->getTransform();
}

void WasdMover::update(float deltaTime) {

	// Set the mouse to relative mode when the right mouse button is down
	if (m_mouse->isKeyDownFrame(2)) {
		m_mouse->setRelativeMode(SDL_TRUE);
	}
	else if (m_mouse->isKeyUpFrame(2)) {
		m_mouse->setRelativeMode(SDL_FALSE);
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
		glm::ivec2 diff = m_mouse->getDifferential();
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