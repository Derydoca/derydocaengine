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
		m_yawSensitivity(0.005f)
	{
	}

	void WasdControlStrategy::update(const float deltaTime, std::shared_ptr<Components::Transform> cameraTransform)
	{
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
				glm::vec4 globalMomentum = cameraTransform->getModel() * glm::vec4(localMomentum, 0);

				// Translate
				cameraTransform->setPos(cameraTransform->getPos() + glm::vec3(globalMomentum));

			} // End keyboard movement

			{ // Mouse look
				glm::ivec2 diff = m_mouse->getRelativeMovement();
				m_pitch -= (float)diff.y * m_pitchSensitivity;
				m_yaw -= (float)diff.x * m_yawSensitivity;

				if (m_pitch < m_minPitch)
				{
					m_pitch = m_minPitch;
				}
				else if (m_pitch > m_maxPitch)
				{
					m_pitch = m_maxPitch;
				}

				glm::fquat newQuat =
					glm::rotate(m_yaw, glm::vec3(0, 1, 0))
					*
					glm::rotate(m_pitch, glm::vec3(1, 0, 0))
					;
				cameraTransform->setQuat(newQuat);
			} // End mouse look
		}
	}

}
