#pragma once
#include "SceneCameraInputStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	class WasdControlStrategy : public SceneCameraInputStrategy
	{
	public:
		WasdControlStrategy();

		virtual void updateActiveState(bool isViewportHovered) override;
		virtual void updateCameraTransform(std::shared_ptr<Components::Transform> cameraTransform) override;
		virtual bool updateInput(const float deltaTime, bool isViewportHovered) override;
	private:
		float m_moveSpeed;
		float m_pitch;
		float m_pitchSensitivity;
		float m_minPitch;
		float m_maxPitch;
		float m_yaw;
		float m_yawSensitivity;
		glm::vec3 m_localTranslationDelta;
	};

}
