#pragma once
#include "sdl2\SDL.h"

namespace DerydocaEngine::Input
{

	struct Key
	{
	public:
		Key()
		{
			m_isDown = false;
			m_downTime = 0;
		}
		~Key() {}

		inline bool isDown() const { return m_isDown; }
		void setState(bool const& isDown, unsigned long const& tick) {

			if (m_prevDownState != isDown)
			{
				m_stateChangeTick = tick;
			}
			m_prevDownState = m_isDown;
			m_isDown = isDown;
		}

		unsigned long getStateChangeTick() { return m_stateChangeTick; }
	private:
		unsigned int m_downTime;
		bool m_isDown;
		bool m_prevDownState;
		unsigned long m_stateChangeTick = 0;
	};

}
