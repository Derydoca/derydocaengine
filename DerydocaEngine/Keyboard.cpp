#include "Keyboard.h"



Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

void Keyboard::init()
{
	SDL_GetKeyboardState(NULL);
}

void Keyboard::update()
{
	m_tick++;
}

bool Keyboard::isKeyDown(int keycode)
{	
	if (keycode >= m_numkeys) {
		return false;
	}
	return m_keys[keycode].isDown();
}

bool Keyboard::isKeyDownFrame(int keycode)
{
	if (keycode >= m_numkeys) {
		return false;
	}
	Key key = m_keys[keycode];
	return key.isDown() && key.getStateChangeTick() == m_tick;
}

bool Keyboard::isKeyUpFrame(int keycode)
{
	if (keycode >= m_numkeys) {
		return false;
	}
	Key key = m_keys[keycode];
	return !key.isDown() && key.getStateChangeTick() == m_tick;
}

void Keyboard::setKeyState(int keycode, bool isDown)
{
	if (keycode >= m_numkeys) {
		return;
	}
	m_keys[keycode].setState(isDown, m_tick);
}
