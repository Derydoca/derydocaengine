#include "InputManager.h"

InputManager::InputManager()
{

	m_keyboard = new Keyboard();
	m_keyboard->init();

	m_mouse = new Mouse();

}

InputManager::~InputManager()
{
	delete m_keyboard;
	delete m_mouse;
}
