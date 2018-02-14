#pragma once

#include <sdl2/SDL.h>
#undef main
#include <string>
#include "Keyboard.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	virtual ~Display();

	void init(int width, int height, const std::string& title);
	void update();
	bool isClosed();
	inline float getAspectRatio() { return (float)m_width / (float)m_height; }

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }

	void bindAsRenderTarget();
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

