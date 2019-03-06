#include "EditorComponentsPch.h"
#include "SceneCameraInputStrategy.h"

namespace DerydocaEngine::Components::SceneCameraInput
{

	SceneCameraInputStrategy::SceneCameraInputStrategy() :
		m_isActive(false),
		m_keyboard(0),
		m_mouse(0)
	{
		m_keyboard = Input::InputManager::getInstance().getKeyboard();
		m_mouse = Input::InputManager::getInstance().getMouse();
	}

	SceneCameraInputStrategy::~SceneCameraInputStrategy()
	{
		m_keyboard = nullptr;
		m_mouse = nullptr;
	}

}
