#pragma once

namespace DerydocaEngine::Rendering::Gui
{

	class DearImgui {
	public:
		static ImGuiIO init(SDL_Window* window, SDL_GLContext& context);
		static void newFrame(SDL_Window* window);
		static void render(SDL_Window* window, SDL_GLContext& context);
		static void shutdown();
		static bool processEvent(SDL_Event& event);
	};

}