#pragma once
#include "Components\Transform.h"
#include "Input\InputManager.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	class SceneCameraInputStrategy
	{
	public:
		virtual void updateActiveState(bool isViewportHovered) = 0;
		virtual void updateCameraTransform(std::shared_ptr<Components::Transform> cameraTransform) = 0;
		virtual bool updateInput(const float deltaTime, bool isViewportHovered) = 0;

		bool isActive() const { return m_isActive; };
	protected:
		SceneCameraInputStrategy();
		virtual ~SceneCameraInputStrategy();

		void setActive(const bool isActive) { m_isActive = isActive; }
	protected:
		Input::Keyboard* m_keyboard;
		Input::Mouse* m_mouse;
	private:
		bool m_isActive;
	};

}
