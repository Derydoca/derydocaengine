#include "Input\Key.h"

#include "sdl2\SDL.h"

namespace DerydocaEngine::Input
{

	Key::Key()
	{
		m_isDown = false;
		m_downTime = 0;
	}

	Key::~Key()
	{
	}

	void Key::setState(bool const& isDown, unsigned long const& tick) {

		if (m_prevDownState != isDown)
		{
			m_stateChangeTick = tick;
		}
		m_prevDownState = m_isDown;
		m_isDown = isDown;
	}

}
