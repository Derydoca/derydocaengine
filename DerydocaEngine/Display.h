#pragma once

#include <sdl2/SDL.h>
#undef main
#include <string>
#include "Keyboard.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);
	virtual ~Display();

	void clear(float r, float g, float b, float a);
	void update();
	bool isClosed();
	inline float getAspectRatio() { return (float)m_width / (float)m_height; }
	inline void setKeyboard(Keyboard* keyboard) { m_keyboard = keyboard; }

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
	int m_width;
	int m_height;
	Keyboard* m_keyboard;
};

