#pragma once
#include <sdl2\SDL.h>
#include "Keyboard.h"
#include "Display.h"
#include "GameComponent.h"

class ScreenshotUtil : public GameComponent
{
public:
	GENINSTANCE(ScreenshotUtil);
	ScreenshotUtil();
	ScreenshotUtil(Display* const& display, Keyboard* const& keyboard);
	~ScreenshotUtil();

	void postRender();
private:
	Keyboard* m_keyboard;
	Display* m_display;
};

