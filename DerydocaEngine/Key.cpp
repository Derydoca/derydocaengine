#include "Key.h"


Key::Key()
{
	m_isDown = false;
	m_downTime = 0;
}

void Key::KeyDown()
{
	m_isDown = true;
	m_downTime = SDL_GetTicks();
}

void Key::KeyUp()
{
	m_isDown = false;
}

Key::~Key()
{
}
