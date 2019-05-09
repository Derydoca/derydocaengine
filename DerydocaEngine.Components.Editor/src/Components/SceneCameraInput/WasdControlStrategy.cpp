#include "EditorComponentsPch.h"
#include "WasdControlStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{
	WasdControlStrategy::WasdControlStrategy() :
		SceneCameraInputStrategy(),
		m_moveSpeed(5.0f),
		m_pitch(0.0f),
		m_pitchSensitivity(0.005f),
		m_minPitch(-glm::half_pi<float>()),
		m_maxPitch(glm::half_pi<float>()),
		m_yaw(0.0f),
		m_yawSensitivity(0.005f),
		m_localTranslationDelta(glm::vec3(0))
	{
	}

	void WasdControlStrategy::updateActiveState(bool isViewportHovered)
	{
		if (isViewportHovered && m_mouse->isKeyDownFrame(2))
		{
			m_mouse->setRelative(true);
			setActive(true);
		}
		if (isActive() && !m_mouse->isKeyDown(2))
		{
			m_mouse->setRelative(false);
			setActive(false);
		}
	}

	void WasdControlStrategy::updateCameraTransform(std::shared_ptr<Components::Transform> cameraTransform)
	{
		// Convert the local translation delta vector to a global translation vector and move the camera by that vector
		glm::vec3 globalTranslationDelta = cameraTransform->getModel() * glm::vec4(m_localTranslationDelta, 0);
		cameraTransform->setPos(cameraTransform->getPos() + globalTranslationDelta);

		// Calculate the rotation quaternion using the yaw and pitch and set the camera's rotation quaternion to that
		glm::fquat newQuat =
			glm::rotate(m_yaw, glm::vec3(0, 1, 0))
			*
			glm::rotate(m_pitch, glm::vec3(1, 0, 0))
			;
		cameraTransform->setQuat(newQuat);
	}

	bool WasdControlStrategy::updateInput(const float deltaTime, bool isViewportHovered)
	{
		// Only manipulate the variables when the right mouse button is pressed
		if (m_mouse->isKeyDown(2))
		{
			// Reset the translation delta
			m_localTranslationDelta = glm::vec3();

			// Scale the movement speed by the delta time
			float moveSpeed = m_moveSpeed * deltaTime;

			// Z axis translation
			if (m_keyboard->isKeyDown(SDLK_w)) {
				m_localTranslationDelta.z -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_s)) {
				m_localTranslationDelta.z += moveSpeed;
			}

			// X axis translation
			if (m_keyboard->isKeyDown(SDLK_a)) {
				m_localTranslationDelta.x -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_d)) {
				m_localTranslationDelta.x += moveSpeed;
			}

			// Y axis translation
			if (m_keyboard->isKeyDown(SDLK_q)) {
				m_localTranslationDelta.y -= moveSpeed;
			}
			if (m_keyboard->isKeyDown(SDLK_e)) {
				m_localTranslationDelta.y += moveSpeed;
			}

			// Convert the mouse's relative movement into pitch and
			//  yaw and add the delta to the existing values
			glm::ivec2 diff = m_mouse->getRelativeMovement();
			m_pitch -= (float)diff.y * m_pitchSensitivity;
			m_yaw -= (float)diff.x * m_yawSensitivity;

			// Constrain the pitch between the min and max values
			if (m_pitch < m_minPitch)
			{
				m_pitch = m_minPitch;
			}
			else if (m_pitch > m_maxPitch)
			{
				m_pitch = m_maxPitch;
			}

			// Assume something has changed
			return true;
		}

		// If we are here, then there were no possible changes to the values
		return false;
	}

}
