#pragma once
#include "Components\GameComponent.h"

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

	class ScreenshotUtil : public GameComponent, SelfRegister<ScreenshotUtil>
	{
	public:
		GENINSTANCE(ScreenshotUtil);
		ScreenshotUtil();
		~ScreenshotUtil();

		void postRender();
	private:
		Input::Keyboard* m_keyboard;
		Rendering::Display* m_display;
	};

}
