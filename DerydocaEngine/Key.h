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
	inline bool isDownFrame() { return m_isDown && !m_prevDownState; }
	inline bool isUpFrame() { return !m_isDown && m_prevDownState; }
	void setState(bool isDown) {
		m_prevDownState = m_isDown;
		m_isDown = isDown;

		if (m_isDown != m_prevDownState) {
			if (isDown) {
				keyDown();
			}
			else {
				keyUp();
			}
		}
	}
private:
	unsigned int m_downTime;
	bool m_isDown;
	bool m_prevDownState;
};

