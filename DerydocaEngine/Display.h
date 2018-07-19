#pragma once

#include <sdl2/SDL.h>
#undef main
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "Keyboard.h"

class Camera;

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
	void registerCamera(Camera* cam) { m_camera = cam; }
	void unregisterCamera() { m_camera = nullptr; }
private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}

	void windowSizeChanged(int width, int height);

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
	int m_width;
	int m_height;
	Keyboard* m_keyboard;
	Camera* m_camera;
};

