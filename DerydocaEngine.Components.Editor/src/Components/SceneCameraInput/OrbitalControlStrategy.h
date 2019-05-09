#pragma once
#include "SceneCameraInputStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	class OrbitalControlStrategy : public SceneCameraInputStrategy
	{
	public:
		OrbitalControlStrategy();

		virtual void updateActiveState(bool isViewportHovered) override;
		virtual void updateCameraTransform(std::shared_ptr<Components::Transform> cameraTransform) override;
		virtual bool updateInput(const float deltaTime, bool isViewportHovered) override;
	private:
		const float MIN_DISTANCE = 0.01f;

		glm::vec3 m_offset;
		float m_distance;
		float m_zoomFactor;
		float m_pitch;
		float m_pitchSensitivity;
		float m_minPitch;
		float m_maxPitch;
		float m_yaw;
		float m_yawSensitivity;
		glm::vec2 m_translationDelta;
		float m_translationSpeed;
	};

}
