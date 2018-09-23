#pragma once
#include "GameComponent.h"

namespace DerydocaEngine {
	namespace Input {
		class Keyboard;
	}
	namespace Rendering {
		class Display;
	}
}

namespace DerydocaEngine::Components
{

	class ScreenshotUtil : public GameComponent
	{
	public:
		GENINSTANCE(ScreenshotUtil);
		ScreenshotUtil();
		ScreenshotUtil(Rendering::Display* const& display, Input::Keyboard* const& keyboard);
		~ScreenshotUtil();

		void postRender();
	private:
		Input::Keyboard* m_keyboard;
		Rendering::Display* m_display;
	};

}
