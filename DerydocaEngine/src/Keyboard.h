#pragma once
#include "Key.h"

namespace DerydocaEngine::Input
{

	class Keyboard
	{
	public:
		Keyboard();
		~Keyboard();

		void init();
		void update();
		bool isKeyDown(int const& keycode) const;
		bool isKeyDownFrame(int const& keycode) const;
		bool isKeyUpFrame(int const& keycode) const;
		void setKeyState(int const& keycode, bool const& isDown);
	private:
		const int m_numkeys = 512;
		Key m_keys[512];
		unsigned long m_tick = 0;
	};

}
