#pragma once
#include <sdl2\SDL.h>
#include "SOIL.h"
#include "Keyboard.h"
#include "Display.h"
#include "GameComponent.h"
#include <chrono>
#include <ctime>

class ScreenshotUtil : public GameComponent
{
public:
	GENINSTANCE(ScreenshotUtil);
	ScreenshotUtil();
	ScreenshotUtil(Display* display, Keyboard* keyboard);
	~ScreenshotUtil();

	void postRender();
private:
	Keyboard* m_keyboard;
	Display* m_display;
};

