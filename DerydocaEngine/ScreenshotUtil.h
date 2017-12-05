#pragma once
#include <sdl2\SDL.h>
#include "SOIL.h"
#include "Keyboard.h"
#include "Display.h"
#include <chrono>
#include <ctime>

class ScreenshotUtil
{
public:
	ScreenshotUtil(Display* display, Keyboard* keyboard);
	~ScreenshotUtil();

	void update();
private:
	Keyboard* m_keyboard;
	Display* m_display;
};

