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
	bool isKeyDown(int keycode);
	void setKeyState(int keycode, bool isDown);
private:
	const int m_numkeys = 512;
	Key m_keys[512];
};

