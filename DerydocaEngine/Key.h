#pragma once
#include "sdl2\SDL.h"

struct Key
{
public:
	Key()
	{
		m_isDown = false;
		m_downTime = 0;
	}
	~Key() {}

	inline bool isDown() { return m_isDown; }
	void setState(bool isDown, unsigned long tick) {
		m_prevDownState = m_isDown;
		m_isDown = isDown;
		m_stateChangeTick = tick;
	}

	unsigned long getStateChangeTick() { return m_stateChangeTick; }
private:
	unsigned int m_downTime;
	bool m_isDown;
	bool m_prevDownState;
	unsigned long m_stateChangeTick = 0;
};

