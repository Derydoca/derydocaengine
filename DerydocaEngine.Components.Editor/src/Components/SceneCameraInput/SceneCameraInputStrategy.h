#pragma once
#include "Components\Transform.h"
#include "Input\InputManager.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	class SceneCameraInputStrategy
	{
	public:
		virtual void update(const float deltaTime, std::shared_ptr<Components::Transform> cameraTransform) = 0;
	protected:
		SceneCameraInputStrategy();
		virtual ~SceneCameraInputStrategy();
	protected:
		Input::Keyboard* m_keyboard;
		Input::Mouse* m_mouse;
	};

}
