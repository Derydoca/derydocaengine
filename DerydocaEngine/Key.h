#pragma once
#include "sdl2\SDL.h"

struct Key
{
public:
	Key();
	~Key();

	void keyDown();
	void keyUp();
	inline bool isDown() { return m_isDown; }
	inline float downTime() { return (float)(SDL_GetTicks() - m_downTime) / 1000.0f; }
private:
	unsigned int m_downTime;
	bool m_isDown;
};

