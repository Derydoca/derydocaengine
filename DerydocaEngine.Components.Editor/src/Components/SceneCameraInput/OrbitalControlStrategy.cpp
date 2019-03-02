#include "EditorComponentsPch.h"
#include "OrbitalControlStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	OrbitalControlStrategy::OrbitalControlStrategy() :
		SceneCameraInputStrategy(),
		m_distance(5.0f),
		m_zoomSpeed(5.0f),
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

	}

}
