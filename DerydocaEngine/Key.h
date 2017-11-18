#pragma once
#include "sdl2\SDL.h"

struct Key
{
public:
	Key();
	void KeyDown();
	void KeyUp();
	inline bool IsDown() { return m_isDown; }
	inline float DownTime() { return (float)(SDL_GetTicks() - m_downTime) / 1000.0f; }
	~Key();
private:
	unsigned int m_downTime;
	bool m_isDown;
};

