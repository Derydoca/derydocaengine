#pragma once
#include <vector>
#include "Keyboard.h"
#include "Mouse.h"

class InputManager
{
public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}

	Keyboard* getKeyboard() { return m_keyboard; }
	Mouse* getMouse() { return m_mouse; }

	void operator=(InputManager const&) = delete;
private:
	InputManager();
	~InputManager();

	InputManager(InputManager const&);

	Keyboard* m_keyboard;
	Mouse* m_mouse;
};

