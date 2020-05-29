#include "EditorComponentsPch.h"
#include "OrbitalControlStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	OrbitalControlStrategy::OrbitalControlStrategy() :
		SceneCameraInputStrategy(),
		m_offset(0.0f),
		m_distance(5.0f),
		m_zoomFactor(0.1f),
		m_pitch(0.0f),
		m_pitchSensitivity(0.005f),
		m_minPitch(-glm::half_pi<float>()),
		m_maxPitch(glm::half_pi<float>()),
		m_yaw(0.0f),
		m_yawSensitivity(0.005f),
		m_translationDelta(0.0f),
		m_translationSpeed(0.001f)
	{
	}

	void OrbitalControlStrategy::updateActiveState(bool isViewportHovered)
	{
		if (isViewportHovered && (m_mouse->isKeyDownFrame(0) || m_mouse->isKeyDownFrame(2)))
		{
			m_mouse->setRelative(true);
			setActive(true);
		}
		if (isActive() && (!m_mouse->isKeyDown(0) && !m_mouse->isKeyDown(2)))
		{
			m_mouse->setRelative(false);
			setActive(false);
		}
	}

	void OrbitalControlStrategy::updateCameraTransform(std::shared_ptr<Components::Transform> cameraTransform)
	{
		// Update the offset based on the translation amount
		glm::vec3 globalTranslationDelta = cameraTransform->getModel() * glm::vec4(m_translationDelta.x, m_translationDelta.y, 0.0f, 0.0f);
		m_offset += globalTranslationDelta;
		m_translationDelta.x = 0;
		m_translationDelta.y = 0;

		// Calculate the new position of the camera by factoring in the distance away from the model
		//  along with the pitch, yaw and offset values
		glm::vec3 pos = glm::vec3(0, 0, 1) * m_distance;
		pos = glm::rotate(pos, m_pitch, glm::vec3(1, 0, 0));
		pos = glm::rotate(pos, m_yaw, glm::vec3(0, 1, 0));
		pos += m_offset;
		cameraTransform->setPosition(pos);

		// Calculate the new rotation quaternion of the camera by factoring in the pitch and yaw
		glm::fquat newQuat =
			glm::rotate(m_yaw, glm::vec3(0, 1, 0))
			*
			glm::rotate(m_pitch, glm::vec3(1, 0, 0))
			;
		cameraTransform->setQuaternion(newQuat);
	}

	bool OrbitalControlStrategy::updateInput(const float deltaTime, bool isViewportHovered)
	{
		if (isActive())
		{
			// Translate the camera when the left mouse button is down
			if (m_mouse->isKeyDown(0))
			{
				// Get the mouse's movement since the last frame
				glm::ivec2 diff = m_mouse->getRelativeMovement();

				m_translationDelta.x = diff.x * -m_translationSpeed * m_distance;
				m_translationDelta.y = diff.y * m_translationSpeed * m_distance;

				// Assume something has changed
				return true;
			}
			// Orbit the camera when the right mouse button is down
			else if (m_mouse->isKeyDown(2))
			{
				// Get the mouse's movement since the last frame
				glm::ivec2 diff = m_mouse->getRelativeMovement();

				// Convert the movement into a change in pitch and yaw
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

				// Constrain the distance of the camera from the origin so that we can not pass through the origin
				if (m_distance < MIN_DISTANCE)
				{
					m_distance = MIN_DISTANCE;
				}

				// Assume something has changed
				return true;
			}
		}

		if (isViewportHovered)
		{
			int zoomWheelY = m_mouse->getRelativeWheelMovementY();
			if (zoomWheelY)
			{
				m_distance *= 1 + (-zoomWheelY * m_zoomFactor);
				return true;
			}
		}

		// If we are here, then there were no possible changes to the values
		return false;
	}

}
