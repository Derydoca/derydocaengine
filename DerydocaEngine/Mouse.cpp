#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

void Mouse::init()
{
	SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);
	m_prevPos.x = m_currentPos.x;
	m_prevPos.y = m_currentPos.y;
}

void Mouse::update()
{
	m_tick++;

	// Move the last recorded position to the previous position
	m_prevPos.x = m_currentPos.x;
	m_prevPos.y = m_currentPos.y;

	// Query the mouse for it's current position
	unsigned int buttonMask = SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);

	// Update the mouse's button states
	for (int i = 0; i < 5; i++)
	{
		m_keys[i].setState(buttonMask & SDL_BUTTON(i+1), m_tick);
	}
}

bool Mouse::isKeyDown(int const& keycode) const 
{
	if (keycode >= 5) {
		return false;
	}
	return m_keys[keycode].isDown();
}

bool Mouse::isKeyDownFrame(int const& keycode) const 
{
	if (keycode >= 5) {
		return false;
	}
	Key key = m_keys[keycode];
	return key.isDown() && key.getStateChangeTick() == m_tick;
}

bool Mouse::isKeyUpFrame(int const& keycode) const 
{
	if (keycode >= 5) {
		return false;
	}
	Key key = m_keys[keycode];
	return !key.isDown() && key.getStateChangeTick() == m_tick;
}
