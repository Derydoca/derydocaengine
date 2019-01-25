#pragma once
#include "Rendering\Display.h"

namespace DerydocaEngine::Rendering::Gui
{

	class DearImgui {
	public:
		static ImGuiIO init(std::shared_ptr<Display> display);
		static void newFrame(std::shared_ptr<Display> display);
		static void render(std::shared_ptr<Display> display);
		static void shutdown();
		static bool processEvent(SDL_Event& event);
	};

}