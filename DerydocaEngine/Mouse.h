#pragma once
#include <glm\glm.hpp>
#include <sdl2\SDL.h>
#include "Key.h"

class Mouse
{
public:
	Mouse();
	~Mouse();
	void init();
	void update();
	Key* getKey(int keyIndex) { return &m_keys[keyIndex]; }
	void setRelativeMode(SDL_bool enabled) { SDL_SetRelativeMouseMode(enabled); }
	bool isKeyDown(int keycode);
	bool isKeyDownFrame(int keycode);
	bool isKeyUpFrame(int keycode);

	inline glm::ivec2 getDifferential() { return m_currentPos - m_prevPos; }
private:
	glm::ivec2 m_currentPos;
	glm::ivec2 m_prevPos;
	Key m_keys[5];
	unsigned long m_tick;
};

