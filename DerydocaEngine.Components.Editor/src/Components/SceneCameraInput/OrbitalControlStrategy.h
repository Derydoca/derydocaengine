#pragma once
#include "SceneCameraInputStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	class OrbitalControlStrategy : public SceneCameraInputStrategy
	{
	public:
		OrbitalControlStrategy();
		virtual void update(const float deltaTime, std::shared_ptr<Components::Transform> cameraTransform) override;
	private:
		float m_distance;
		float m_zoomSpeed;
		float m_pitch;
		float m_pitchSensitivity;
		float m_minPitch;
		float m_maxPitch;
		float m_yaw;
		float m_yawSensitivity;
	};

}
