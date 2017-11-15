#pragma once

#include <sdl2/SDL.h>
#undef main
#include <string>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	void Clear(float r, float g, float b, float a);
	void Update();
	bool IsClosed();
	inline float GetAspectRatio() { return (float)m_width / (float)m_height; }
	virtual ~Display();
private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
	int m_width;
	int m_height;
};

