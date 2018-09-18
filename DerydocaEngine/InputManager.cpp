#include "InputManager.h"

namespace DerydocaEngine::Input
{

	InputManager::InputManager()
	{

		m_keyboard = new Keyboard();
		m_keyboard->init();

		m_mouse = new Mouse();
		m_mouse->init();

	}

	InputManager::~InputManager()
	{
		delete m_keyboard;
		delete m_mouse;
	}

}
