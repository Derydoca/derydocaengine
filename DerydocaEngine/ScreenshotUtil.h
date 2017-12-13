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
	ScreenshotUtil(Display* display, Keyboard* keyboard);
	~ScreenshotUtil();

	void update(float deltaTime);
	void render(Camera* camera, MatrixStack* matrixStack);
	void postRender();
private:
	Keyboard* m_keyboard;
	Display* m_display;
};

