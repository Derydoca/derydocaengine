#include "Input\InputManager.h"

namespace DerydocaEngine::Input
{

	InputManager::InputManager()
	{
		m_keyboard.init();
		m_mouse.init();
	}

	InputManager::~InputManager()
	{
	}

}
