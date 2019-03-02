#include "EditorComponentsPch.h"
#include "OrbitalControlStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	OrbitalControlStrategy::OrbitalControlStrategy() :
		SceneCameraInputStrategy(),
		m_distance(5.0f),
		m_zoomFactor(0.1f),
		m_pitch(0.0f),
		m_pitchSensitivity(0.005f),
		m_minPitch(-glm::half_pi<float>()),
		m_maxPitch(glm::half_pi<float>()),
		m_yaw(0.0f),
		m_yawSensitivity(0.005f)
	{
	}

	void OrbitalControlStrategy::update(const float deltaTime, std::shared_ptr<Components::Transform> cameraTransform)
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

			int zoomWheelY = m_mouse->getRelativeWheelMovementY();
			if (zoomWheelY)
			{
				m_distance *= 1 + (zoomWheelY * m_zoomFactor);
			}

			// Constrain the distance of the camera from the origin so that we can not pass through the origin
			if (m_distance < MIN_DISTANCE)
			{
				m_distance = MIN_DISTANCE;
			}

			// Calculate the new position of the camera by factoring in the distance away from the model
			//  along with the pitch and yaw values
			glm::vec3 pos = glm::vec3(0, 0, 1) * m_distance;
			pos = glm::rotate(pos, m_pitch, glm::vec3(1, 0, 0));
			pos = glm::rotate(pos, m_yaw, glm::vec3(0, 1, 0));
			cameraTransform->setPos(pos);

			// Calculate the new rotation quaternion of the camera by factoring in the pitch and yaw
			glm::fquat newQuat =
				glm::rotate(m_yaw, glm::vec3(0, 1, 0))
				*
				glm::rotate(m_pitch, glm::vec3(1, 0, 0))
				;
			cameraTransform->setQuat(newQuat);
		}
	}

}
