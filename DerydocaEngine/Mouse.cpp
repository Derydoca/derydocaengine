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
	//SDL_BUTTON_LEFT
}

void Mouse::update()
{
	// Move the last recorded position to the previous position
	m_prevPos.x = m_currentPos.x;
	m_prevPos.y = m_currentPos.y;

	// Query the mouse for it's current position
	SDL_GetMouseState(&m_currentPos.x, &m_currentPos.y);
}
