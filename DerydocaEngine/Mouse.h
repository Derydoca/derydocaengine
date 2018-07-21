#pragma once
#include <glm/glm.hpp>
#include <sdl2/SDL.h>
#include "Key.h"

class Mouse
{
public:
	Mouse();
	~Mouse();
	void init();
	void update();
	Key* getKey(int keyIndex) { return &m_keys[keyIndex]; }
	void setRelative(bool isRelative)
	{
		// Set the mouse to relative mode and flush the first relative move so that the first query isn't incorrect
		SDL_SetRelativeMouseMode((SDL_bool)isRelative);
		if (isRelative) {
			SDL_GetRelativeMouseState(nullptr, nullptr);
		}
	}
	bool isKeyDown(int keycode);
	bool isKeyDownFrame(int keycode);
	bool isKeyUpFrame(int keycode);

	inline glm::ivec2 getDifferential() {
		if (m_isLocked)
		{
			return m_currentPos - m_lockPos;
		}
		else
		{
			return m_currentPos - m_prevPos;
		}
	}

	inline glm::ivec2 getRelativeMovement() {
		glm::ivec2 diff;
		SDL_GetRelativeMouseState(&diff.x, &diff.y);
		return diff;
	}
private:
	glm::ivec2 m_currentPos;
	glm::ivec2 m_prevPos;
	glm::ivec2 m_lockPos;
	Key m_keys[5];
	unsigned long m_tick;
	bool m_isLocked = false;
};

