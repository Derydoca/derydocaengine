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
		SERIALIZE_FUNC_DEFINITIONS;
		ScreenshotUtil();
		~ScreenshotUtil();

		void postRender();
		void saveScreenshot();

	private:
		Input::Keyboard* m_Keyboard;
		Rendering::Display* m_Display;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::ScreenshotUtil, 0);
