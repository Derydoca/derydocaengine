#include "Key.h"


Key::Key()
{
	m_isDown = false;
	m_downTime = 0;
}

void Key::keyDown()
{
	m_isDown = true;
	m_downTime = SDL_GetTicks();
}

void Key::keyUp()
{
	m_isDown = false;
}

Key::~Key()
{
}
