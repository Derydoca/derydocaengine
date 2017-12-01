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
	// Move the last recorded position to the previous position
	m_prevPos.x = m_currentPos.x;
	m_prevPos.y = m_currentPos.y;

	// Query the mouse for it's current position
	unsigned int buttonMask = SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);

	for (int i = 0; i < 5; i++)
	{
		m_keys[i].setState(buttonMask & SDL_BUTTON(i+1));
	}
}
