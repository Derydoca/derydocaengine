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

	//m_keys = new Key[m_numkeys];
}

void Keyboard::update()
{
	SDL_PumpEvents();
	const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
	bool bIsDown = keyboardState[98] != 0;
	for(int i = 0; i < m_numkeys; i++) {
		m_keys[i].setState(keyboardState[i]);
	}
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
	m_keys[keycode].isDownFrame();
}

bool Keyboard::isKeyUpFrame(int keycode)
{
	if (keycode >= m_numkeys) {
		return false;
	}
	m_keys[keycode].isUpFrame();
}

void Keyboard::setKeyState(int keycode, bool isDown)
{
	if (keycode >= m_numkeys) {
		return;
	}
	m_keys[keycode].setState(isDown);
}
