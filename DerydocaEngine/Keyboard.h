#pragma once
#include "Key.h"
#include <sdl2\SDL.h>
#include <iostream>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	void init();
	void update();
	bool isKeyDown(int const& keycode);
	bool isKeyDownFrame(int const& keycode);
	bool isKeyUpFrame(int const& keycode);
	void setKeyState(int const& keycode, bool const& isDown);
private:
	const int m_numkeys = 512;
	Key m_keys[512];
	unsigned long m_tick = 0;
};

