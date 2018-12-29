#pragma once

#include <sdl2\SDL.h>

#include <vendor/imgui/imgui_impl_sdl.h>
#include <vendor/imgui/imgui_impl_opengl3.h>

namespace DerydocaEngine::Rendering::Gui
{

	class GuiImpl {
	public:
		static ImGuiIO init(SDL_Window* window, SDL_GLContext* context);
		static void newFrame(SDL_Window* window);
		static void render();
		static void shutdown();
	};

}